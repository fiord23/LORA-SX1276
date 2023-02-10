/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SPI1.h"
#include "LORA SX1276.h" 
#include "Commands.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define RX_BUFFER_SIZE 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
   uint8_t rx_buffer_len;
   bool flag_press_button = false;
   bool receiver_answer = false;
   bool flag_uart_receiver = false;
   bool auto_send = false;
   uint8_t num_of_bytes;
   uint8_t str_uart[RX_BUFFER_SIZE] = {1, 2, 3};
   uint8_t str_uart_r[RX_BUFFER_SIZE] = {0, };
   uint8_t str_test[3] = {0, '\r', '\n'};
   uint8_t counter_led = 0;
   uint32_t counter_for_button = 0;
   uint8_t button_sw = '0';
   bool button_state = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
   HAL_Init();
   SystemClock_Config();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
    
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
    
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  SPI_GPIO_config ();
  SPI_config();
  LED_config(); 
  exti_config();
  MCO_config();
  Lora_init ();
  Lora_Show_Firmware_Version(); 
  Lora_Show_Help();
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
  HAL_UART_Receive_IT(&huart2, str_uart, RX_BUFFER_SIZE);
  HAL_TIM_OC_Start_IT(&htim10, TIM_CHANNEL_1);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t autosend_i;
  while (1)
  {    
    
    if (flag_press_button)
     {
         Lora_Show_Firmware_Version();
         HAL_Delay(100);
       // Lora_transmit (str_uart, rx_buffer_len);
//        auto_send=!auto_send;
        flag_press_button = false;          
     }
    
     if (flag_uart_receiver)
     {
       if(!Parser_Commands())
       {
//         setendstr(str_uart);
         Lora_transmit (str_uart, rx_buffer_len);       
       }
//       flag_transmit = false; 
       flag_uart_receiver = false;
     }   
    if (receiver_answer)
      {
        Lora_recieve(str_uart_r, &num_of_bytes);
        HAL_UART_Transmit(&huart2, str_uart_r, num_of_bytes, 30);
        Show_RSSI();
        Show_SNR();
        led_red_high();
        receiver_answer = false;
      }    
 
    if (auto_send) {
      HAL_UART_Transmit(&huart2, "Start AutoSend\r\n", 16, 30);  
      uint8_t msg[] = "Msg N00000 by LoRa fw 000000\r\n";
      uint8_t msg_size=strlen((char *)msg);
      dec2str(autosend_i++,&msg[5],5);
      HAL_UART_Transmit(&huart2, "send:", 5, 30);  
      HAL_UART_Transmit(&huart2, msg, msg_size, 30);  
      Lora_transmit (msg, msg_size);
      HAL_Delay(2500);
      if (autosend_i==32000) {
        auto_send=false;
        HAL_UART_Transmit(&huart2, "finish autosend.\r\n", 18, 30);
      }

    }
  //  */
     
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void EXTI0_IRQHandler(void)	
{
    
    if( (GPIOA->IDR & (1 << 0)) == 1 ) 
    {
      EXTI->PR = EXTI_PR_PR0;
      button_state = true;
    }
    else 
    {
      EXTI->PR = EXTI_PR_PR0;
      if ((counter_for_button >0)  &  (counter_for_button <10) )
       {
        led_blue_low();
        led_green_low();
      }       
      else if ((counter_for_button >=10)  &  (counter_for_button <=500) )
      {
        led_blue_high();
        led_green_low();
      }
        counter_for_button = 0;
        button_state = false;
      
    }
    
   // flag_press_button = true;
   // auto_send=!auto_send;
}

void EXTI15_10_IRQHandler(void)	
{
    EXTI->PR = EXTI_PR_PR15;
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    receiver_answer = true;
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
        if(htim->Instance == TIM10) //check if the interrupt comes from TIM1
        {
          if(button_state)
          {
            counter_for_button++;
          }
          if(counter_for_button >= 2000)
            counter_for_button = 2000;
           if ((counter_for_button >1500)  &  (counter_for_button <=2000) )
            {
                led_blue_low();
                led_green_high();
            }             
        }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
