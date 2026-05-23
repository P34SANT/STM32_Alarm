//COMMANDS.C for custom cmds

#include "commands.h"
#include "alarm.h"


void alarm_shutdown(void){
                      
   xEventGroupSetBits(alarm_events , ALARM_OFF_BIT );
   xEventGroupClearBits(alarm_events , ALARM_ON_BIT );
   
}


void alarm_fire(void){
  if(f_armed){
     xEventGroupSetBits(alarm_events , ALARM_ON_BIT );
     xEventGroupClearBits(alarm_events , ALARM_OFF_BIT );
  }
  else{terminal_write_string("fired but is disarmed\r\n");}
}


void siren_enable (void){
  f_siren_enabled = 1;

}

void siren_disable (void){
  f_siren_enabled = 0;

}

void arm(void){
    f_armed = 1;
  
}
void disarm(void){
    f_armed = 0;
  
}

void status (void){

  terminal_write_string("sensors 1      :"); terminal_write_num(f_sensor1_enabled); terminal_write_string("\r\n");
  terminal_write_string("sensors 2      :"); terminal_write_num(f_sensor2_enabled); terminal_write_string("\r\n");
  terminal_write_string("sensors 3      :"); terminal_write_num(f_sensor3_enabled); terminal_write_string("\r\n");
  
  terminal_write_string("siren          :"); terminal_write_num(f_siren_enabled); terminal_write_string("\r\n");
  terminal_write_string("is armed       :"); terminal_write_num(f_armed);         terminal_write_string("\r\n");
  terminal_write_string("auto shut down :"); terminal_write_num(f_auto_shutdown); terminal_write_string("\r\n");
  if(f_auto_shutdown)
  terminal_write_string("shut down timer:"); terminal_write_num((uint32_t)auto_shutdown_second); terminal_write_string("\r\n");
  terminal_write_string("poll interval  :"); terminal_write_num((uint32_t)poll_interval_ms);  terminal_write_string("\r\n");


}
