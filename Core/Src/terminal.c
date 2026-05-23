//terminal.c
#include "terminal.h"
#include <stdio.h>
#include <string.h>

TaskHandle_t usart1_parser_handle;
TaskHandle_t usart1_transmitter_handle;
QueueHandle_t usart1_parser_queue;
QueueHandle_t usart1_transmitter_queue;
SemaphoreHandle_t usart1_mutex;

void terminal_help (void){
     xSemaphoreTake(usart1_mutex , portMAX_DELAY);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"-------\r\n" , 9 , 200);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"turn off alarm  : 0\r\n"  , 20 , 200);
 
     HAL_UART_Transmit(&huart1 , (uint8_t *)"turn on alarm   : 1\r\n"  , 20 , 200);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"mointoring on   : 2\r\n"  , 20 , 200);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"mointoring off  : 3\r\n"  , 20 , 200);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"siren enable    : 4\r\n"  , 20 , 200);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"siren disable   : 5\r\n"  , 20 , 200);
     
     HAL_UART_Transmit(&huart1 , (uint8_t *)"-------\r\n" , 9 , 200);
     
     xSemaphoreGive(usart1_mutex);

}


void terminal_write_num(uint32_t num)
{
    char buffer[20] = "";

    int len = snprintf(buffer, sizeof(buffer), "%u", num);

    for(int i = 0; i < len; i++)
    {
        xQueueSend(usart1_transmitter_queue, &buffer[i], portMAX_DELAY);
    }
}

void terminal_write_string(const char *str)
{
    int len = strlen(str);

    for(int i = 0; i < len; i++)
    {
        xQueueSend(usart1_transmitter_queue, &str[i], portMAX_DELAY);
    }
}


void usart1_transmitter (void* params){

	uint8_t receivedByte;

	BaseType_t qStatus;
	while(1){
                 xSemaphoreTake(usart1_mutex , portMAX_DELAY);

          
		qStatus = xQueueReceive(usart1_transmitter_queue , &receivedByte , portMAX_DELAY);
		if(qStatus != pdPASS){
			HAL_UART_Transmit(&huart1 , (uint8_t *)"usart1 transmitter failed\n\r" , 27, 100 );
		}else{

                        HAL_UART_Transmit(&huart1 , &receivedByte , sizeof(uint8_t) , 10);

                  xSemaphoreGive(usart1_mutex);




		}

	}//end while

}//end task

void usart1_parser (void* params){

	uint8_t receivedByte;

	BaseType_t qStatus;
	while(1){
		qStatus = xQueueReceive(usart1_parser_queue , &receivedByte , portMAX_DELAY);
		if(qStatus != pdPASS){
			HAL_UART_Transmit(&huart1 , (uint8_t *)"usart1 parser failed\n\r" , 22, 100 );
		}else{

			if      (receivedByte == '1'){
                          
                                alarm_fire();
                                
			}else if(receivedByte == '0'){
                          
                                alarm_shutdown();
				
			}else if(receivedByte == '2'){
                          
                          f_monitor = 1;
                       
			}else if(receivedByte == '3'){
                          
                          f_monitor = 0;
                          
                        }else if(receivedByte == '4'){
                          
                          siren_enable();
                          
                        }else if(receivedByte == '5'){
                          
                          siren_disable();
                          
			}else {
                          
                          terminal_help();
                        }





		}

	}//end while

}//end task