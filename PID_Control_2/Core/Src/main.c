/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CAN_receive.h"
#include "pid_control.h"
#include "bsp_can.h"
#include "serialplot.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t exti_flag=0;
float output=0;
float speed_target=0;
uint32_t counter=0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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
  counter=0;//初始化计数器
  
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
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_UART_Init(&huart6);
	can_filter_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		switch(exti_flag){
			case 0:HAL_GPIO_WritePin(GPIOH,GPIO_PIN_12,GPIO_PIN_RESET);
      CAN_cmd_chassis(output,0,0,0);
      HAL_Delay(10);break;
		  case 1:HAL_GPIO_WritePin(GPIOH,GPIO_PIN_12,GPIO_PIN_SET);
      CAN_cmd_chassis(0,0,0,0);
		}//按键控制电机启动停止
    speed_target=8000*sin(2*3.14159*((float)counter)/100.0f);//目标速度设为正弦波变化
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1) // 定时器触发发送
  {
     if (!serialplot_dma_busy)
     {
        UART_TX_DATA[2 + 2 * 3 + 0] = (int8_t)((int16_t)speed_target & 0xFF);
        UART_TX_DATA[2 + 2 * 3 + 1] = (int8_t)((int16_t)speed_target >> 8 & 0xFF);   
			 int8_t *buf=get_UART_TX_DATA_ptr();
			
         uint16_t len = get_UART_TX_DATA_size();
			   HAL_UART_Transmit_DMA(&huart6,buf,len);
         if (HAL_UART_Transmit_DMA(&huart6, buf, len) == HAL_OK)
         {
             serialplot_dma_busy = 1; // 置忙，等待 HAL_UART_TxCpltCallback 清零
         }
         else
         {
             // 发送失败，可根据需要处理错误或重试
         }
     }
  }
  if(htim->Instance==TIM2){//计时器2中断，PID计算
  HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_11);

   static PID_Controller pid1;//速度 PID控制器实例
   static PID_Controller pid2;//扭矩 PID控制器实例

   PID_Init_speed(&pid1, 1.0f, 1.0f, 0.1f);// 初始化PID控制器, 设置Kp, Ki, Kd, 积分限幅和误差限幅
   const motor_measure_t *motor = get_chassis_motor_measure_point(0);
   int16_t measured_value = motor->speed_rpm; // 获取实际速度
   float output_current = PID_Compute_speed(&pid1, speed_target,(float)measured_value, 1000.0f, 1000.0f);

  PID_Init_torque(&pid2, 0.5f, 0.0f, 0.05f);
   int16_t measured_value2 = motor->given_current; // 获取实际电流
   float output_current2 = PID_Compute_torque(&pid2, output_current, (float)measured_value2, 100.0f, 10.0f); // ,获取实际电流值用于输出
   output=output_current2;
  }
  if(htim->Instance==TIM3){//定时器3中断
    counter++;
     }
}
//0 e=8000,p=1,i=0,d=0.05,


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_0:
		{
	  exti_flag=(exti_flag+1)%2;
		}break;
		default:break;
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
