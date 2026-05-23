#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void alarm_shutdown(void);

void alarm_fire(void);

void siren_enable (void);

void status (void);

void siren_disable (void);

void arm(void);

void disarm(void);
#ifdef __cplusplus
}
#endif
#endif /*__ COMMANDS_H__ */

