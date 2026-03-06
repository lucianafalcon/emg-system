/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : task_menu.c
  * @brief          : Interactive state machine for EMG acquisition, calibration,
  *          LCD interface and UART communication
  ******************************************************************************

/********************** inclusions *******************************************/
#include "main.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"
#include "app.h"
#include "task_menu_attribute.h"
#include "task_menu_interface.h"
#include "display.h"

/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI          0ul
#define G_TASK_MEN_TICK_CNT_INI     0ul
#define CALIB_BUZZ_TICKS            300ul
#define CALIB_REST_TICKS            10000ul
#define CALIB_CONTRACT_TICKS        10000ul

#define BUZZER_ON()     HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET)
#define BUZZER_OFF()    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET)

/* Envio USART3 por registros */
static void uart3_send(const char *buf, uint16_t len)
{
	 /* Debug: verificar USART3 */
	    LOGGER_INFO("UART3 CR1=%lX SR=%lX", USART3->CR1, USART3->SR);

    if (!(USART3->CR1 & USART_CR1_UE)) return; /* USART3 no habilitado */
    for (uint16_t i = 0; i < len; i++)
    {
        uint32_t timeout = 10000;
        while (!(USART3->SR & USART_SR_TXE) && timeout--);
        if (timeout == 0) return; /* timeout - no bloquear */
        USART3->DR = (uint8_t)buf[i];
    }
}

/********************** internal data ****************************************/
task_menu_dta_t task_menu_dta = {0, ST_IDLE, EV_NONE, false};

void task_menu_statechart(void);
void LCD_show(const char *first_row, const char *second_row);

const char *p_task_menu  = "Task Menu (EMG Interactive Menu)";
const char *p_task_menu_ = "Non-Blocking & Update By Time Code";

uint32_t g_task_menu_cnt;
volatile uint32_t g_task_menu_tick_cnt;

static uint32_t calib_rest_sum     = 0;
static uint32_t calib_rest_count   = 0;
static uint32_t calib_cont_sum     = 0;
static uint32_t calib_cont_count   = 0;

uint32_t calib_emg_rest       = 0;
uint32_t calib_emg_active     = 0;
uint32_t calib_threshold      = 0;
uint32_t calib_threshold_high = 0;
uint32_t calib_threshold_low  = 0;

/********************** functions ********************************************/

void LCD_show(const char *first_row, const char *second_row)
{
    displayCharPositionWrite(0, 0);
    displayStringWrite("                    ");
    displayCharPositionWrite(0, 1);
    displayStringWrite("                    ");
    displayCharPositionWrite(0, 0);
    displayStringWrite(first_row);
    displayCharPositionWrite(0, 1);
    displayStringWrite(second_row);
}

void task_menu_init(void *parameters)
{
    task_menu_dta_t *p_task_menu_dta;
    task_menu_st_t   state;
    task_menu_ev_t   event;
    bool             b_event;

    LOGGER_INFO(" ");
    LOGGER_INFO("  %s is running - %s", GET_NAME(task_menu_init), p_task_menu);
    LOGGER_INFO("  %s is a %s", GET_NAME(task_menu), p_task_menu_);

    g_task_menu_cnt = G_TASK_MEN_CNT_INI;
    LOGGER_INFO("   %s = %lu", GET_NAME(g_task_menu_cnt), g_task_menu_cnt);

    init_queue_event_task_menu();

    p_task_menu_dta = &task_menu_dta;
    state = ST_IDLE;       p_task_menu_dta->state = state;
    event = EV_NONE;       p_task_menu_dta->event = event;
    b_event = false;       p_task_menu_dta->flag  = b_event;
    p_task_menu_dta->tick  = 0;

    LOGGER_INFO(" ");
    LOGGER_INFO("   %s = %lu   %s = %lu   %s = %s",
                GET_NAME(state),   (uint32_t)state,
                GET_NAME(event),   (uint32_t)event,
                GET_NAME(b_event), (b_event ? "true" : "false"));

    BUZZER_OFF();

    /* Init LCD */
    HAL_Delay(500);
    displayInit(DISPLAY_CONNECTION_GPIO_4BITS);
    HAL_Delay(100);
    LCD_show("EMG System Ready", "Press ENTER/CALIB");

    /* USART3 en PC10(TX)/PC11(RX) con remap parcial */
    __HAL_RCC_AFIO_CLK_ENABLE();
    AFIO->MAPR = (AFIO->MAPR & ~(0x3U << 4)) | (0x1U << 4);  /* remap parcial USART3 */

    __HAL_RCC_USART3_CLK_ENABLE();

    /* PC10 = TX: AF Push-Pull 2MHz */
    GPIOC->CRH = (GPIOC->CRH & ~(0xFU << 8))  | (0xAU << 8);
    /* PC11 = RX: Input floating */
    GPIOC->CRH = (GPIOC->CRH & ~(0xFU << 12)) | (0x4U << 12);

    /* 9600 baud con PCLK1=32MHz */
    USART3->BRR =  0x1A0B;  /* 4800 baud a 32MHz */
    USART3->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void task_menu_update(void *parameters)
{
    bool b_time_update_required = false;

    __asm("CPSID i");
    if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
    {
        g_task_menu_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    while (b_time_update_required)
    {
        g_task_menu_cnt++;
        if (task_menu_dta.tick > 0) task_menu_dta.tick--;
        task_menu_statechart();

        __asm("CPSID i");
        if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
        {
            g_task_menu_tick_cnt--;
            b_time_update_required = true;
        }
        else b_time_update_required = false;
        __asm("CPSIE i");
    }
}

void task_menu_statechart(void)
{
    task_menu_dta_t *p = &task_menu_dta;

    if (any_event_task_menu())
    {
        p->event = get_event_task_menu();
        p->flag  = true;
    }

    switch (p->state)
    {
        case ST_IDLE:
            if (p->flag && p->event == EV_BTN_ENTER)
            {
                p->flag  = false;
                p->state = ST_ACQUIRING;
                LCD_show("EMG Running...", "STOP to finish");
            }
            else if (p->flag && p->event == EV_BTN_CALIB)
            {
                p->flag  = false;
                p->state = ST_CALIB_BUZZ1;
                BUZZER_ON();
                p->tick  = CALIB_BUZZ_TICKS;
                LCD_show("Calibrating...", "Get ready!");
            }
            break;

        case ST_ACQUIRING:
        {
            static uint32_t lcd_refresh_cnt = 0;
            static uint32_t emg1 = 0;
            static uint32_t emg2 = 0;
            static uint8_t  adc_ch = 0;
            static uint32_t buf1[8] = {0};
            static uint32_t buf2[8] = {0};
            static uint8_t  buf_idx = 0;
            static bool muscle1_active = false;
            static bool muscle2_active = false;

            if (ADC1->SR & ADC_SR_EOC)
            {
                if (adc_ch == 0) { buf1[buf_idx] = ADC1->DR; adc_ch = 1; }
                else             { buf2[buf_idx] = ADC1->DR; buf_idx = (buf_idx + 1) % 8; adc_ch = 0; }

                uint32_t sum1 = 0, sum2 = 0;
                for (uint8_t i = 0; i < 8; i++) { sum1 += buf1[i]; sum2 += buf2[i]; }
                emg1 = sum1 / 8;
                emg2 = sum2 / 8;

                if (!muscle1_active && emg1 > calib_threshold_high) muscle1_active = true;
                if ( muscle1_active && emg1 < calib_threshold_low)  muscle1_active = false;
                if (!muscle2_active && emg2 > calib_threshold_high) muscle2_active = true;
                if ( muscle2_active && emg2 < calib_threshold_low)  muscle2_active = false;
            }

            lcd_refresh_cnt++;
            if (lcd_refresh_cnt >= 1000)
            {
                lcd_refresh_cnt = 0;

                const char *estado1 = calib_threshold_high > 0 ? (muscle1_active ? "ACT" : "REL") : "---";
                const char *estado2 = calib_threshold_high > 0 ? (muscle2_active ? "ACT" : "REL") : "---";

                char row1[21], row2[21];
                snprintf(row1, sizeof(row1), "E1:%4lu %s", emg1, estado1);
                snprintf(row2, sizeof(row2), "E2:%4lu %s", emg2, estado2);
                LCD_show(row1, row2);

                /* Enviar al ESP32 */
                char uart_buf[32];
                uint16_t len = snprintf(uart_buf, sizeof(uart_buf), "%lu,%lu,%s,%s\n",
                                        emg1, emg2, estado1, estado2);
                uart3_send(uart_buf, len);
            }

            if (p->flag && p->event == EV_BTN_STOP)
            {
                p->flag = false; p->state = ST_IDLE;
                lcd_refresh_cnt = 0; adc_ch = 0; buf_idx = 0;
                muscle1_active = false; muscle2_active = false;
                for (uint8_t i = 0; i < 8; i++) { buf1[i] = 0; buf2[i] = 0; }
                LCD_show("EMG Stopped", "Press ENTER/CALIB");
            }
            break;
        }

        case ST_CALIB_BUZZ1:
            if (p->tick == 0)
            {
                BUZZER_OFF(); p->state = ST_CALIB_REST; p->tick = CALIB_REST_TICKS;
                calib_rest_sum = 0; calib_rest_count = 0;
                LCD_show("RELAX muscle", "Relax: 10 sec");
            }
            break;

        case ST_CALIB_REST:
        {
            static uint32_t prev_sec_rest = 0xFF;
            if (ADC1->SR & ADC_SR_EOC) { calib_rest_sum += ADC1->DR; calib_rest_count++; }

            uint32_t curr_sec = (p->tick + 999) / 1000;
            if (curr_sec != prev_sec_rest)
            {
                prev_sec_rest = curr_sec;
                char row[20];
                snprintf(row, sizeof(row), "Relax: %lu sec", curr_sec);
                LCD_show("RELAX muscle", row);
            }
            if (p->tick == 0)
            {
                if (calib_rest_count > 0) calib_emg_rest = calib_rest_sum / calib_rest_count;
                prev_sec_rest = 0xFF; p->state = ST_CALIB_BUZZ2; BUZZER_ON(); p->tick = CALIB_BUZZ_TICKS;
                LCD_show("CONTRACT muscle", "Get ready!");
            }
            break;
        }

        case ST_CALIB_BUZZ2:
            if (p->tick == 0)
            {
                BUZZER_OFF(); p->state = ST_CALIB_CONTRACT; p->tick = CALIB_CONTRACT_TICKS;
                calib_cont_sum = 0; calib_cont_count = 0;
                LCD_show("CONTRACT muscle", "Cont: 10 sec");
            }
            break;

        case ST_CALIB_CONTRACT:
        {
            static uint32_t prev_sec_contract = 0xFF;
            if (ADC1->SR & ADC_SR_EOC) { calib_cont_sum += ADC1->DR; calib_cont_count++; }

            uint32_t curr_sec = (p->tick + 999) / 1000;
            if (curr_sec != prev_sec_contract)
            {
                prev_sec_contract = curr_sec;
                char row[20];
                snprintf(row, sizeof(row), "Cont: %lu sec", curr_sec);
                LCD_show("CONTRACT muscle", row);
            }
            if (p->tick == 0)
            {
                if (calib_cont_count > 0) calib_emg_active = calib_cont_sum / calib_cont_count;
                uint32_t rango = (calib_emg_active > calib_emg_rest) ? (calib_emg_active - calib_emg_rest) : 0;
                calib_threshold_high = calib_emg_rest + (rango * 75) / 100;
                calib_threshold_low  = calib_emg_rest + (rango * 25) / 100;
                calib_threshold      = calib_threshold_high;
                prev_sec_contract = 0xFF; p->state = ST_CALIB_BUZZ3; BUZZER_ON(); p->tick = CALIB_BUZZ_TICKS;
                LCD_show("Calib complete!", "");
            }
            break;
        }

        case ST_CALIB_BUZZ3:
            if (p->tick == 0)
            {
                BUZZER_OFF(); p->state = ST_IDLE;
                LCD_show("Calib Done!", "Press ENTER/CALIB");
            }
            break;

        default:
            BUZZER_OFF(); p->tick = 0; p->flag = false;
            p->state = ST_IDLE; p->event = EV_NONE;
            LCD_show("EMG System Ready", "Press ENTER/CALIB");
            break;
    }
}

/********************** end of file ******************************************/
