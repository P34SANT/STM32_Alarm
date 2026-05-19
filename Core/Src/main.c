
#include <string.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "gpio.h"
#include "sensors.h"



const Config_t g_default_config =
{
    .magic = CONFIG_MAGIC,
    .version = CONFIG_VERSION,
    .length = sizeof(Config_t),

    .arm_state = 0,
    .sensor1_enabled = 1,
    .sensor2_enabled = 1,
    .sensor3_enabled = 1,
    .siren_enabled = 1,

    .boot_grace_s = 10,
    .sensor_grace_ms = 200,
    .sensor_check_ms = 1000,

    .call_number = "0912345678",

    .owner_numbers =
    {
        "0912345678",
        "0999999999",
        "",
        "",
        ""
    },

    .crc32 = 0
};



HAL_StatusTypeDef st;

void SystemClock_Config(void);
void StartDefaultTask(void *argument);
void LED_task(void *argument);
void SENSOR_task(void *argument);

TaskHandle_t defaultTaskHandle;
TaskHandle_t led_task_handle;
TaskHandle_t sensor_task_handle;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  if (xTaskCreate(StartDefaultTask, "default", 128, NULL, 0, &defaultTaskHandle) != pdPASS)
  {
    Error_Handler();
  }

  if (xTaskCreate(LED_task, "led", 128, NULL, 1, &led_task_handle) != pdPASS)
  {
    Error_Handler();
  }

  if (xTaskCreate(SENSOR_task, "sensor", 256, NULL, 1, &sensor_task_handle) != pdPASS)
  {
    Error_Handler();
  }

  
  
  
  
  
  vTaskStartScheduler();

  Error_Handler();
  
  
  
  
}



void LED_task(void *argument)
{
  volatile char str[] = "" ;
  while(1)
  {
   // HAL_UART_Receive_DMA(&huart1 , (uint8_t *)str , 16);
  //  HAL_UART_Transmit_DMA(&huart1 , (uint8_t *)str , 16);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  
}






void SENSOR_task(void *argument){
  
  const char *str = "is active\n\r" ;
  const char *str1 = "isnt active\n\r" ;
  uint8_t sensors_state  = 0;
  
  //TickType_t NOW = 0; 
  
  while(1){
    
     sensors_state = read_sensors();
     if(sensors_state == 1){
                  vTaskDelay(pdMS_TO_TICKS(200));
                  sensors_state = read_sensors();
                  if(sensors_state == 1){
                    
                          st = HAL_UART_Transmit_DMA(&huart1, (uint8_t *)str, 11);
       
                  }else{
                          st = HAL_UART_Transmit_DMA(&huart1 , (uint8_t *)str1 , 13  ); 
     }
     }else{
     
     st = HAL_UART_Transmit_DMA(&huart1 , (uint8_t *)str1 , 13  ); 
     
    
     
     }
     
     vTaskDelay(pdMS_TO_TICKS(1000));
    
    
  }
  
  
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

//default task
void StartDefaultTask(void *argument)
{
  /* Infinite loop */
  for(;;)
  {
    
  }
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
