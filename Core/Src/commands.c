//COMMANDS.C for custom cmds

#include "commands.h"
#include "alarm.h"


void alarm_shutdown(void){
                      
   xEventGroupSetBits(alarm_events , ALARM_OFF_BIT );
   xEventGroupClearBits(alarm_events , ALARM_ON_BIT );
   
}


void alarm_fire(void){

     xEventGroupSetBits(alarm_events , ALARM_ON_BIT );
     xEventGroupClearBits(alarm_events , ALARM_OFF_BIT );

}