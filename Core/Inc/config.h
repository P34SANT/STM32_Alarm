#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

  
extern uint16_t debounce_time_ms ;
extern uint16_t poll_interval_ms;


extern uint8_t f_auto_shutdown ;
extern uint8_t f_siren_enabled ;
extern uint8_t auto_shutdown_second ;
extern uint8_t f_armed;

extern uint8_t f_monitor ;
#ifdef __cplusplus
}
#endif
#endif /*__CONFIG_H__ */

