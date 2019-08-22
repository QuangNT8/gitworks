#include "stm32f103xb.h"
#include "controller.h"

#define sysclk  48000000
#define baud    115200

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            (((_PCLK_)*25U)/(4U*(_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        (((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            (((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U)) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))


void uart::Controller::initUART_()
{
#if 1
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // USART1 clock enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOB->CRL |= GPIO_CRL_CNF7_1|GPIO_CRL_CNF6_1|GPIO_CRL_MODE6;
//      __HAL_AFIO_REMAP_USART1_ENABLE();
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
      /* USER CODE END USART1_Init 1 */
    USART1->CR1 |=  USART_CR1_TE | USART_CR1_RE;

    USART1->BRR = UART_BRR_SAMPLING16(sysclk, baud);
    // Default value
    USART1->CR2 = (uint32_t)(0x00000000);
    USART1->CR3 = (uint32_t)(0x00000000);
    USART1->CR1 |= USART_CR1_UE;
#endif
}


bool uart::Controller::rxReady_()
{
    return (USART1->SR & USART_SR_RXNE);
}

bool uart::Controller::txReady_()
{
    return (USART1->SR & USART_SR_TXE) ;
}

uint8_t uart::Controller::read_()
{
    return (uint8_t)USART1->DR;
}

void uart::Controller::write_(uint8_t c)
{
    USART1->DR = c;
}
