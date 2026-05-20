
#include <string.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "gpio.h"
#include "sensors.h"
#include "semphr.h"
#include "event_groups.h"

#define ALARM_ON_BIT      (1U<<0)
#define ALARM_OFF_BIT     (1U<<3)

//temp flags must be replaced with config struct
uint8_t f_auto_shutdown      = 0;
uint8_t f_siren_enabled      = 1;
uint8_t auto_shutdown_second = 5;



TimerHandle_t auto_shutdown_timer;
HAL_StatusTypeDef st;

void SystemClock_Config(void);
void StartDefaultTask(void *argument);
void LED_task(void *argument);
void sensor_task(void *argument);
void alarm_task(void *argument);
void usart1_parser (void* params);
void auto_shutdown_callBack(TimerHandle_t auto_shutdown_timer);

TaskHandle_t alarm_task_handle;
TaskHandle_t sensor_task_handle;
TaskHandle_t usart1_parser_handle;

QueueHandle_t usart1_queue;

EventGroupHandle_t alarm_events;
EventBits_t waitBitsResult; // for alarm task debugging
uint8_t idletest = 0 ;




int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  
  usart1_queue = xQueueCreate( 50 , sizeof(uint8_t));
  
  alarm_events = xEventGroupCreate(); 
  
  auto_shutdown_timer = xTimerCreate("autoshutdown timer" , auto_shutdown_second * 1000 , pdFALSE , NULL , auto_shutdown_callBack);
  

  if (xTaskCreate(alarm_task , "siren ", 512, NULL, 2, &alarm_task_handle) != pdPASS)
  {
    Error_Handler();
  }

  if (xTaskCreate(sensor_task, "sensor", 512, NULL, 1, &sensor_task_handle) != pdPASS)
  {
    Error_Handler();
  }
  if (xTaskCreate(usart1_parser, "u1 parser ", 512, NULL, 3, &usart1_parser_handle) != pdPASS)
  {
    Error_Handler();
  }

  HAL_GPIO_WritePin(LED_GPIO_Port   , LED_Pin   , GPIO_PIN_SET);
  
  
  
  
  vTaskStartScheduler();

  Error_Handler();
  
  
  
  
}





void sensor_task(void *argument){
  

  uint8_t sensors_state  = 0;
  

  
  while(1){
    
     sensors_state = read_sensors();
     if(sensors_state == 1){
                  vTaskDelay(pdMS_TO_TICKS(200));
                  sensors_state = read_sensors();
                  if(sensors_state == 1){
                    
                          
                          xEventGroupSetBits(alarm_events , ALARM_ON_BIT );
                          xEventGroupClearBits(alarm_events , ALARM_OFF_BIT );
       
                  }else{
                          
     }
     }else{
     
    
     
    
     
     }
     
     vTaskDelay(pdMS_TO_TICKS(1000));
    
    
  }
  
  
}

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
                    HAL_UART_Transmit(&huart1 , (uint8_t *)"Siren onn\n\r" , 11 , 100 );
       }
                    HAL_UART_Transmit(&huart1 , (uint8_t *)"Alarm onn\n\r" , 11 , 100 );
                    state = 1;
     }
                  
   }else{
     if(state == 1){
     HAL_GPIO_WritePin(LED_GPIO_Port   , LED_Pin   , GPIO_PIN_SET);
     HAL_GPIO_WritePin(siren_GPIO_Port , siren_Pin , GPIO_PIN_RESET); 
     HAL_UART_Transmit(&huart1 , (uint8_t *)"Alarm off\n\r" , 11 , 100 );
     state = 0;
     xEventGroupClearBits(alarm_events , ALARM_OFF_BIT );
     }
   }
   vTaskDelay(pdMS_TO_TICKS(500));
  }//end while
  
}//end task

void usart1_parser (void* params){

	uint8_t receivedByte;

	BaseType_t qStatus;
	while(1){
		qStatus = xQueueReceive(usart1_queue , &receivedByte , portMAX_DELAY);
		if(qStatus != pdPASS){
			HAL_UART_Transmit_DMA(&huart1 , (uint8_t *)"usart1 parser failed\n\r" , 22 );
		}else{

			if(receivedByte == '1'){
				 xEventGroupSetBits(alarm_events , ALARM_ON_BIT );
                                 xEventGroupClearBits(alarm_events , ALARM_OFF_BIT );

			}else if(receivedByte == '0'){
                          xEventGroupSetBits(alarm_events , ALARM_OFF_BIT );
                          xEventGroupClearBits(alarm_events , ALARM_ON_BIT );
                         
				
			}else {
				
			}





		}

	}//end while

}//end task


void auto_shutdown_callBack(TimerHandle_t auto_shutdown_timer){

	HAL_UART_Transmit(&huart1, (uint8_t *)"auto shutdown timer passed\n\r", 28, HAL_MAX_DELAY);
        xEventGroupSetBits(alarm_events , ALARM_OFF_BIT );
        xEventGroupClearBits(alarm_events , ALARM_ON_BIT );
}


void vApplicationIdleHook(){
	idletest++;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	BaseType_t HP_Task_Woken = pdFALSE;

	uint8_t receivedByte = tmpstr;

        if( huart->Instance == USART1){
	xQueueSendFromISR(usart1_queue , &receivedByte , &HP_Task_Woken );
	HAL_UART_Receive_IT(&huart1, &tmpstr , 1);
        }

	portYIELD_FROM_ISR(HP_Task_Woken);
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}




/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  NVIC_SystemReset();
  __disable_irq();
  while (1)
  {
  }
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
