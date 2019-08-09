
#include "osc.h"
#include "stm32f0xx.h"
#include "system_stm32f0xx.h"
#include "stm32f0xx_hal_def.h"
#include "stm32f0xx_hal_rcc_ex.h"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_flash.h"
#include "stm32f0xx_hal_cortex.h"
#include "stm32f0xx_hal.h"

osc::Controller osc::CONTROLLER;



void osc::Controller::init()
{
    HAL_Init();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

      /* System interrupt init*/
      /* SVC_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
      /* PendSV_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
      /* SysTick_IRQn interrupt configuration */
      HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    SystemClock_Config();
}

void osc::Controller::SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
    RCC_OscInitStruct.HSICalibrationValue = 16;
    RCC_OscInitStruct.HSI14CalibrationValue = 16;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;


    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;

    /**Configure the Systick interrupt time
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

HAL_StatusTypeDef osc::Controller::HAL_Init()
{
    HAL_InitTick(TICK_INT_PRIORITY);
      /* Init the low level hardware */
//    HAL_MspInit();
    /* System interrupt init*/
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    /* SVC_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    /* Return function status */
    return HAL_OK;
}

extern "C" void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

void osc::Controller::_Error_Handler(char *file, int line)
{
    /* USER CODE BEGIN Error_Handler_Debug */
      /* User can add his own implementation to report the HAL error return state */
      while(1)
      {
      }
      /* USER CODE END Error_Handler_Debug */
}
