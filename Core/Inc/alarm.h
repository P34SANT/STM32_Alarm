#ifndef __ALARM_H__
#define __ALARM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

//defines
#define ALARM_ON_BIT      (1U<<0)
#define ALARM_OFF_BIT     (1U<<3)

//temp flags must be replaced with config struct
extern uint8_t f_auto_shutdown      ;
extern uint8_t f_siren_enabled      ;
extern uint8_t auto_shutdown_second ;

extern TaskHandle_t alarm_task_handle;
extern TimerHandle_t auto_shutdown_timer;
extern EventGroupHandle_t alarm_events;

void alarm_task(void *argument);
#ifdef __cplusplus
}
#endif
#endif /*__ALARM_H__ */

