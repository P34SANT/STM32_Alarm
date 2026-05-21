#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

  
uint8_t read_sensors(void);
extern TaskHandle_t sensor_task_handle;
void sensor_task(void *argument);


#ifdef __cplusplus
}
#endif
#endif /*__ SENSORS_H__ */

