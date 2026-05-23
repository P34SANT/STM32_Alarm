#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern TaskHandle_t usart1_parser_handle;

extern QueueHandle_t usart1_transmitter_queue;

extern TaskHandle_t usart1_transmitter_handle;;

extern QueueHandle_t usart1_parser_queue;

extern SemaphoreHandle_t usart1_mutex;

void usart1_parser (void* params);

void usart1_transmitter (void* params);

void terminal_write_num(uint32_t num);

void terminal_write_string(const char *str);

void terminal_help (void);

#ifdef __cplusplus
}
#endif
#endif /*__TERMINAL_H__ */

