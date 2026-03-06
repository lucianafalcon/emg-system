#ifndef BOARD_INC_BOARD_H_
#define BOARD_INC_BOARD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/
#define NUCLEO_F103RC		(0)
#define NUCLEO_F303R8		(1)
#define NUCLEO_F401RE		(2)
#define NUCLEO_F446RE		(3)
#define NUCLEO_F413ZH		(4)
#define NUCLEO_F429ZI		(5)
#define NUCLEO_F439ZI		(6)
#define NUCLEO_F767ZI		(7)
#define STM32F407G_DISC1	(8)
#define STM32F429I_DISC1	(9)

#define BOARD (NUCLEO_F103RC)

/* STM32 __Nucleo__ Boards - 32 Pins */
#if (BOARD == NUCLEO_F303R8)

#endif

/* STM32 __Nucleo__ Boards - 64 Pins */
#if ((BOARD == NUCLEO_F103RC) || (BOARD == NUCLEO_F401RE) || (BOARD == NUCLEO_F446RE))

/* --- Original button definitions (kept for compatibility) --- */
#define BTN_A_PIN		B1_Pin
#define BTN_A_PORT		B1_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_RESET
#define BTN_A_HOVER		GPIO_PIN_SET

#define BTN_ENT_PIN		D10_Pin
#define BTN_ENT_PORT	D10_GPIO_Port
#define BTN_ENT_PRESSED	GPIO_PIN_RESET
#define BTN_ENT_HOVER	GPIO_PIN_SET

#define BTN_NEX_PIN		D11_Pin
#define BTN_NEX_PORT	D11_GPIO_Port
#define BTN_NEX_PRESSED	GPIO_PIN_RESET
#define BTN_NEX_HOVER	GPIO_PIN_SET

#define BTN_ESC_PIN		D12_Pin
#define BTN_ESC_PORT	D12_GPIO_Port
#define BTN_ESC_PRESSED	GPIO_PIN_RESET
#define BTN_ESC_HOVER	GPIO_PIN_SET

/* --- EMG System button aliases ---
 *
 *  BTN_ENTER  → D10 (same as BTN_ENT) - Start EMG acquisition
 *  BTN_STOP   → D11 (same as BTN_NEX) - Stop acquisition / stop calibration
 *  BTN_CALIB  → B1  (same as BTN_A, onboard blue button PC13) - Start calibration
 *
 */
#define BTN_ENTER_PIN		BTN_ENT_PIN
#define BTN_ENTER_PORT		BTN_ENT_PORT
#define BTN_ENTER_PRESSED	BTN_ENT_PRESSED

#define BTN_STOP_PIN		BTN_NEX_PIN
#define BTN_STOP_PORT		BTN_NEX_PORT
#define BTN_STOP_PRESSED	BTN_NEX_PRESSED

#define BTN_CALIB_PIN		BTN_ESC_PIN
#define BTN_CALIB_PORT		BTN_ESC_PORT
#define BTN_CALIB_PRESSED	BTN_ESC_PRESSED

/* --- LED --- */
#define LED_A_PIN		LD2_Pin
#define LED_A_PORT		LD2_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

/* --- Buzzer - PA5 (CN10 pin 11, shared with LD2) --- */
#define BUZZER_PIN		LD2_Pin
#define BUZZER_PORT		LD2_GPIO_Port

#endif

/* STM32 __Nucleo__ Boards - 144 Pins */
#if ((BOARD == NUCLEO_F413ZH) || (BOARD == NUCLEO_F429ZI) || (BOARD == NUCLEO_F439ZI) || (BOARD == NUCLEO_F767ZI))

#define BTN_A_PIN		USER_Btn_Pin
#define BTN_A_PORT		USER_Btn_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_SET
#define BTN_A_HOVER		GPIO_PIN_RESET

#define LED_A_PIN		LD1_Pin
#define LED_A_PORT		LD1_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif

/* STM32 Discovery Kits */
#if ((BOARD == STM32F407G_DISC1) || (BOARD == STM32F429I_DISC1))

#define BTN_A_PIN		B1_Pin
#define BTN_A_PORT		B1_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_SET
#define BTN_A_HOVER		GPIO_PIN_RESET

#define LED_A_PIN		LD3_Pin
#define LED_A_PORT		LD3_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_INC_BOARD_H_ */

/********************** end of file ******************************************/
