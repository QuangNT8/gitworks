
#include "gpio.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_gpio_ex.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal.h"

#include "stm32f1xx_hal_can.h"
#include "../uart/controller.h"

#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define LD4_Pin GPIO_PIN_8
#define LD4_GPIO_Port GPIOC
#define LD3_Pin GPIO_PIN_9
#define LD3_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA

GPIO::Controller GPIO::CONTROLLER;




void GPIO::Controller::init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void GPIO::Controller::loop()
{
    static uint8_t i=0;
    static uint32_t counterLed = 0;
    if(counterLed++ > (1 << 13))
    {
//        printf("quang %d\r\n",i++);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        counterLed = 0;
    }
    //    HAL_Delay(1);
}

void GPIO::Controller::led_PB1_toggle()
{
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}
