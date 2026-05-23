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