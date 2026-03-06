#ifndef TASK_INC_TASK_MENU_ATTRIBUTE_H_
#define TASK_INC_TASK_MENU_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include <stdint.h>
#include <stdbool.h>

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/* Menu Statechart - State Transition Table
 *
 * 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 * 	| Current               | Event                 | [Guard]               | Next                  | Actions               |
 * 	| State                 |                       |                       | State                 |                       |
 * 	|=======================+=======================+=======================+=======================+=======================|
 * 	| ST_IDLE               | EV_BTN_ENTER          |                       | ST_ACQUIRING          | LCD: "EMG Running"    |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_BTN_CALIB          |                       | ST_CALIB_BUZZ1        | Buzzer ON, tick=BUZZ  |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_ACQUIRING          | EV_BTN_STOP           |                       | ST_IDLE               | LCD: "Stopped"        |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_CALIB_BUZZ1        | (tick == 0)           |                       | ST_CALIB_REST         | Buzzer OFF,           |
 * 	|                       |                       |                       |                       | LCD: "Relax...",      |
 * 	|                       |                       |                       |                       | tick=REST_TIME        |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_CALIB_REST         | (tick == 0)           |                       | ST_CALIB_BUZZ2        | Buzzer ON, tick=BUZZ  |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_CALIB_BUZZ2        | (tick == 0)           |                       | ST_CALIB_CONTRACT     | Buzzer OFF,           |
 * 	|                       |                       |                       |                       | LCD: "Contract!",     |
 * 	|                       |                       |                       |                       | tick=CONTRACT_TIME    |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_CALIB_CONTRACT     | (tick == 0)           |                       | ST_CALIB_BUZZ3        | Buzzer ON, tick=BUZZ  |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_CALIB_BUZZ3        | (tick == 0)           |                       | ST_IDLE               | Buzzer OFF,           |
 * 	|                       |                       |                       |                       | LCD: "Calib Done!"    |
 * 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 */

/* Events that excite Task Menu */
typedef enum task_menu_ev {
    EV_NONE,
    EV_BTN_ENTER,
    EV_BTN_STOP,
    EV_BTN_CALIB
} task_menu_ev_t;

/* States of Task Menu */
typedef enum task_menu_st {
    ST_IDLE,
    ST_ACQUIRING,
    ST_CALIB_BUZZ1,
    ST_CALIB_REST,
    ST_CALIB_BUZZ2,
    ST_CALIB_CONTRACT,
    ST_CALIB_BUZZ3
} task_menu_st_t;

typedef struct {
    uint32_t        tick;       // General purpose countdown tick
    task_menu_st_t  state;
    task_menu_ev_t  event;
    bool            flag;
} task_menu_dta_t;

/********************** external data declaration ****************************/
extern task_menu_dta_t task_menu_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_MENU_ATTRIBUTE_H_ */

/********************** end of file ******************************************/


