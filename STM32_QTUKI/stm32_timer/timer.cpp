#include "timer.h"
#include "stm32f051x8.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_tim.h"

TIM_HandleTypeDef htim1;

timer::Controller timer::CONTROLLER;

void timer::Controller::init(uint32_t tim)
{
    MX_TIMx_Init(TIM1);
}

void timer::Controller::loop()
{

}

void timer::Controller::MX_TIMx_Init(TIM_TypeDef *TIM)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
     TIM_MasterConfigTypeDef sMasterConfig;
     TIM_OC_InitTypeDef sConfigOC;
     TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

     htim1.Instance = TIM;
     htim1.Init.Prescaler = 0;
     htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
     htim1.Init.Period = 480;
     htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
     htim1.Init.RepetitionCounter = 0;
     htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
     if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
     if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC4REF;
     sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
     if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     sConfigOC.OCMode = TIM_OCMODE_PWM1;
     sConfigOC.Pulse = 0;
     sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
     sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
     sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
     sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
     sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
     if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
     sConfigOC.Pulse = 1000;
     sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
     if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
     sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
     sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
     sBreakDeadTimeConfig.DeadTime = 0;
     sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
     sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
     sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
     if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
     {
//       _Error_Handler(__FILE__, __LINE__);
     }

     HAL_TIM_MspPostInit(&htim1);
}

void timer::Controller::HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim)
{
     GPIO_InitTypeDef GPIO_InitStruct;
     if(htim->Instance==TIM1)
     {
     /* USER CODE BEGIN TIM1_MspPostInit 0 */

     /* USER CODE END TIM1_MspPostInit 0 */

       /**TIM1 GPIO Configuration
       PA8     ------> TIM1_CH1
       */
       GPIO_InitStruct.Pin = GPIO_PIN_8;
       GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
       GPIO_InitStruct.Pull = GPIO_NOPULL;
       GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
       GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
       HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

     /* USER CODE BEGIN TIM1_MspPostInit 1 */

     /* USER CODE END TIM1_MspPostInit 1 */
     }
}
