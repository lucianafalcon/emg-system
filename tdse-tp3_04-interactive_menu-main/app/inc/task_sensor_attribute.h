#ifndef TASK_INC_TASK_SENSOR_ATTRIBUTE_H_
#define TASK_INC_TASK_SENSOR_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "task_menu_attribute.h"

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* Sensor Statechart - State Transition Table */
/* 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 * 	| Current               | Event                 |                       | Next                  |                       |
 * 	| State                 | (Parameters)          | [Guard]               | State                 | Actions               |
 * 	|=======================+=======================+=======================+=======================+=======================|
 * 	| INICIAL               |                       |                       | ST_BTN_XX_UP          |                       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_BTN_XX_UP          | EV_BTN_XX_UP          |                       | ST_BTN_XX_UP          |                       |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_BTN_XX_DOWN        |                       | ST_BTN_XX_FALLING     | tick = TICK_MAX       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_BTN_XX_FALLING     | EV_BTN_XX_UP          | [tick == 0]           | ST_BTN_XX_UP          |                       |
 * 	|                       |                       +-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_BTN_XX_FALLING     | tick--                |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_BTN_XX_DOWN        | [tick == 0]           | ST_BTN_XX_DOWN        | put_event_task_menu   |
 * 	|                       |                       |                       |                       |  (signal_down)        |
 * 	|                       |						+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_BTN_XX_FALLING     | tick--                |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 *	| ST_BTN_XX_DOWN        | EV_BTN_XX_UP          |                       | ST_BTN_XX_RISING      | tick = TICK_MAX       |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_BTN_XX_DOWN        |                       | ST_BTN_XX_DOWN        |                       |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_BTN_XX_RISING      | EV_BTN_XX_UP          | [tick == 0]           | ST_BTN_XX_UP          | put_event_task_menu   |
 * 	|                       |                       |                       |                       |  (signal_up)          |
 * 	|                       |						+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_BTN_XX_RISING      | tick--                |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_BTN_XX_DOWN        | [tick == 0]           | ST_BTN_XX_DOWN        |                       |
 * 	|                       |						+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_BTN_XX_RISING      | tick--                |
 * 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 */

/* Events to excite Task Sensor */
typedef enum task_sensor_ev {
    EV_BTN_XX_UP,
    EV_BTN_XX_DOWN
} task_sensor_ev_t;

/* States of Task Sensor */
typedef enum task_sensor_st {
    ST_BTN_XX_UP,
    ST_BTN_XX_FALLING,
    ST_BTN_XX_DOWN,
    ST_BTN_XX_RISING
} task_sensor_st_t;

/* Identifier of Task Sensor - updated to match EMG buttons */
typedef enum task_sensor_id {
    ID_BTN_ENTER,
    ID_BTN_STOP,
    ID_BTN_CALIB
} task_sensor_id_t;

typedef struct {
    task_sensor_id_t    identifier;
    GPIO_TypeDef *      gpio_port;
    uint16_t            pin;
    GPIO_PinState       pressed;
    uint32_t            tick_max;
    task_menu_ev_t      signal_up;      /* fixed: now uses task_menu_ev_t */
    task_menu_ev_t      signal_down;    /* fixed: now uses task_menu_ev_t */
} task_sensor_cfg_t;

typedef struct {
    uint32_t            tick;
    task_sensor_st_t    state;
    task_sensor_ev_t    event;
} task_sensor_dta_t;

/********************** external data declaration ****************************/
extern task_sensor_dta_t task_sensor_dta_list[];

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
