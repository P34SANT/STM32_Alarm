


#include "alarm.h"


//temp flags must be replaced with config struct


TimerHandle_t auto_shutdown_timer;
EventGroupHandle_t alarm_events;
TaskHandle_t alarm_task_handle;




void alarm_task(void *argument){
  
  uint8_t state = 0;
  
  
  
  while(1){
   
   waitBitsResult = xEventGroupWaitBits(alarm_events , ALARM_ON_BIT | ALARM_OFF_BIT , pdFALSE , pdFALSE , portMAX_DELAY ) ;
   if((ALARM_ON_BIT & waitBitsResult) ){
     if(state == 0){
       if(f_auto_shutdown){xTimerStart(auto_shutdown_timer , 0 );}
       if(f_siren_enabled){
                    HAL_GPIO_WritePin(siren_GPIO_Port , siren_Pin , GPIO_PIN_SET) ;
                    HAL_GPIO_WritePin(LED_GPIO_Port   , LED_Pin   , GPIO_PIN_RESET) ;
                    terminal_write_string("siren on \r\n" );
       }
                    terminal_write_string("alarm on \r\n" );
                    state = 1;
     }
                  
   }else{
     if(state == 1){
     HAL_GPIO_WritePin(LED_GPIO_Port   , LED_Pin   , GPIO_PIN_SET);
     HAL_GPIO_WritePin(siren_GPIO_Port , siren_Pin , GPIO_PIN_RESET); 
     terminal_write_string("alarm off \r\n" );
     state = 0;
     xEventGroupClearBits(alarm_events , ALARM_OFF_BIT );
     }
   }
   vTaskDelay(pdMS_TO_TICKS(500));
  }//end while
  
}//end task