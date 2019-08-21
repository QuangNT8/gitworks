#include "stdio.h"
#include <stdint.h>
#include <string.h>
#include  <errno.h>
#include  <sys/unistd.h>
#include "Usart.h"
#include "stm32f0xx_hal_uart.h"
#include "stm32f0xx_hal_uart_ex.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_tim.h"

FILE __stdout;
FILE __stdin;

static UART_HandleTypeDef huart1;

#if 1
extern "C" int fputc(int ch, FILE * p_file)
{
    uint8_t c[1];
    c[0] = ch & 0x00FF;
    HAL_UART_Transmit(&huart1, &*c, 1, 10);
    return ch;
}
extern "C" int __putchar(int ch, FILE * p_file)
{
    uint8_t c[1];
    c[0] = ch & 0x00FF;
    HAL_UART_Transmit(&huart1, &*c, 1, 10);
    return ch;
}

extern "C" int _write(int file,char *data, int len)
{

    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
      errno = EBADF;
      return -1;
    }
    // arbitrary timeout 1000
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 1000);
    // return # of bytes written - as best we can tell
    return (status == HAL_OK ? len : 0);
}
#endif




USART::Controller USART::CONTROLLER;

void USART::Controller::init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

   __HAL_RCC_USART1_CLK_ENABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    HAL_UART_Init(&huart1);

}
void USART::Controller::loop()
{
    static uint32_t counterLed = 0;
    static uint32_t i=0;
    static uint8_t datatest[10];
    uint8_t c[1];
//    printf("hello...\r\n");
    if(counterLed++ > (1 << 18))
    {
        printf("hello...%d\r\n",i++);
//        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
//        c[0] = 'a' & 0x00FF;
//        HAL_UART_Transmit(&huart1, &*c, 1, 10);
        counterLed = 0;
    }
}

void USART::Controller::myprintf(const char *fmt,...)
{
//    va_list argp;
//    va_start(argp, fmt);
//    vprint(fmt, argp);
//    va_end(argp);
}

void USART::Controller::vprint(const char *fmt, char * argp)
{
//    char string[200];
//    va_list argptr;
//    va_start(argptr, argp);
//    if(0 < vsprintf(string,fmt,argp))
//    {

//    }
}
