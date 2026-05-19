#include "sensors.h"







volatile GPIO_PinState sensor1_state = GPIO_PIN_RESET, sensor2_state = GPIO_PIN_RESET , sensor3_state  =GPIO_PIN_RESET ;

void read_sensor_1(void){
  
  sensor1_state =  HAL_GPIO_ReadPin(sensor1_GPIO_Port , sensor1_Pin);
  
  
} 
void read_sensor_2(void){
  
  sensor2_state =  HAL_GPIO_ReadPin(sensor2_GPIO_Port , sensor2_Pin);
  
  
}

void read_sensor_3(void){
  
  sensor3_state = HAL_GPIO_ReadPin(sensor3_GPIO_Port , sensor3_Pin);
    
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