
#include "stm32f103xb.h"


#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_dma_ex.h"
#include "stm32f1xx_hal_uart.h"

#include "uart/controller.h"


#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            (((_PCLK_)*25U)/(4U*(_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        (((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            (((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U)) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

//#include "uart/controller.h"
//#include "Stm32_GPIO/gpio.h"
//#include "controller.h"
//#include "stdio.h"
//#include "CAN/can.h"

//void SystemInit (void)
//{

//};
UART_HandleTypeDef huart1;

int __io_putchar(int ch)
{
    uint8_t c[1];
     c[0] = ch & 0x00FF;
     HAL_UART_Transmit(&huart1, &*c, 1, 10);
     return ch;
}

int _write(int file,char *ptr, int len)
{
     int DataIdx;
     for(DataIdx= 0; DataIdx< len; DataIdx++)
     {
     __io_putchar(*ptr++);
     }
    return len;
}

extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /** NONJTRST: Full SWJ (JTAG-DP + SW-DP) but without NJTRST
  */
  __HAL_AFIO_REMAP_SWJ_NONJTRST();

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
//uint8_t datatest[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};


static void MX_USART1_UART_Init(uint32_t sysclk, uint32_t baud)
{
    uint32_t pclk;
    uint32_t tmpreg;

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // USART1 clock enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOB->CRL |= GPIO_CRL_CNF7_1|GPIO_CRL_CNF6_1|GPIO_CRL_MODE6;
//      __HAL_AFIO_REMAP_USART1_ENABLE();
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
      /* USER CODE END USART1_Init 1 */
    USART1->CR1 |=  USART_CR1_TE | USART_CR1_RE;
    USART1->BRR = UART_BRR_SAMPLING16(sysclk, baud);

    USART1->CR1 |= USART_CR1_UE;

    /* Initialize the UART state */
    huart1.Instance = USART1;
    huart1.ErrorCode = HAL_UART_ERROR_NONE;
    huart1.gState = HAL_UART_STATE_READY;
    huart1.RxState = HAL_UART_STATE_READY;
}

void SystemClock_Config(void)
{
      RCC_OscInitTypeDef RCC_OscInitStruct = {0};
      RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
      RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

      /** Initializes the CPU, AHB and APB busses clocks
      */
      RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
      RCC_OscInitStruct.HSIState = RCC_HSI_ON;
      RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
      RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
      RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
      if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      {
        Error_Handler();
      }
      /** Initializes the CPU, AHB and APB busses clocks
      */
      RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
      RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
      RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
      RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
      RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

      if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
      {
        Error_Handler();
      }
      PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
      PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
        Error_Handler();
      }
}

static void MX_GPIO_Init(void)
{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};

//     /* GPIO Ports Clock Enable */
//     __HAL_RCC_GPIOC_CLK_ENABLE();
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     __HAL_RCC_GPIOB_CLK_ENABLE();
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH |= GPIO_CRH_MODE13;
//     /*Configure GPIO pin Output Level */
//     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

//     /*Configure GPIO pin : PC13 */
//     GPIO_InitStruct.Pin = GPIO_PIN_13;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}


int main()
{
    uint8_t datatest[10]= "hello ";
    HAL_Init();
    HAL_MspInit();
    SystemClock_Config();
    MX_GPIO_Init();
    uart::CONTROLLER.init();
    /* Initialize the UART state */
    huart1.Instance = USART1;
    huart1.ErrorCode = HAL_UART_ERROR_NONE;
    huart1.gState = HAL_UART_STATE_READY;
    huart1.RxState = HAL_UART_STATE_READY;
//    MX_USART1_UART_Init(48000000,115200);
    while (true)
    {
        uart::CONTROLLER.printMessage("hello123..");
//        HAL_UART_Transmit(&huart1, datatest, 10, 10);
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        uart::CONTROLLER.loop();
//        HAL_Delay(100);
    }
//    return 0;
}
