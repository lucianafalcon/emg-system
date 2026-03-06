#ifndef TASK_INC_TASK_MENU_INTERFACE_H_
#define TASK_INC_TASK_MENU_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_task_menu(void);
extern void put_event_task_menu(task_menu_ev_t event);
extern task_menu_ev_t get_event_task_menu(void);
extern bool any_event_task_menu(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_MENU_INTERFACE_H_ */

/********************** end of file ******************************************/
