//sensor.c
#include "sensors.h"



TaskHandle_t sensor_task_handle;


volatile GPIO_PinState sensor1_state = GPIO_PIN_RESET, sensor2_state = GPIO_PIN_RESET , sensor3_state  =GPIO_PIN_RESET ;

void read_sensor_1(void){
  
  if(f_sensor1_enabled)
     sensor1_state =  HAL_GPIO_ReadPin(sensor1_GPIO_Port , sensor1_Pin);
  else
     sensor1_state = 1;
  
} 
void read_sensor_2(void){
  
  if(f_sensor2_enabled)
    sensor2_state =  HAL_GPIO_ReadPin(sensor2_GPIO_Port , sensor2_Pin);
  else
    sensor2_state = 1;
    
  
}

void read_sensor_3(void){
  if(f_sensor3_enabled)
      sensor3_state = HAL_GPIO_ReadPin(sensor3_GPIO_Port , sensor3_Pin);
  else 
      sensor3_state = 1;
}

uint8_t read_sensors(void){
  
  
  read_sensor_1();
  read_sensor_2();
  read_sensor_3();
  
  if (sensor1_state == GPIO_PIN_SET && sensor2_state == GPIO_PIN_SET && sensor3_state == GPIO_PIN_SET ){
    
    return 1;
  }else{
    return 0;
    
  }
  
}

//sensor tasks
void sensor_task(void *argument){
  

  uint8_t sensors_state  = 0;
  

  while(1){
    
     sensors_state = read_sensors();
     if(sensors_state == 1){
                  vTaskDelay(pdMS_TO_TICKS(debounce_time_ms));
                  sensors_state = read_sensors();
                  if(sensors_state == 1){
                    
                          alarm_fire();

       
                  }else{ terminal_write_string("debounced\r\n" );  }
     }else{
     

     }
     
     vTaskDelay(pdMS_TO_TICKS(poll_interval_ms));
    
    
  }//end while
  
  
}//end task
