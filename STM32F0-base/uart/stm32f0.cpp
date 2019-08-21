#include "stm32f0xx.h"
#include "controller.h"

void uart::Controller::initUART_()
{
    // GPIOB Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // PB6 and PB7 Alternate function mode
    GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);

    // Set alternate functions AF1 for PB6 and PB7
    GPIOB->AFR[1] |= 0x00000110;

    // USART1 clock enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 115200 Bd @ 48 MHz ???
    // USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
    // BRR[15:4] = USARTDIV[15:4]
    // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
    USART1->BRR = (uint16_t)(0x01A0);

    // USART enable
    // Receiver enable
    // Transmitter enable
    USART1->CR1 = (uint32_t)(USART_CR1_UE |USART_CR1_RE |USART_CR1_TE);

    // Default value
    USART1->CR2 = (uint32_t)(0x00000000);
    USART1->CR3 = (uint32_t)(0x00000000);
}


bool uart::Controller::rxReady_()
{
    return (USART1->ISR & USART_ISR_RXNE);
}

bool uart::Controller::txReady_()
{
    return (USART1->ISR & USART_ISR_TXE) ;
}

uint8_t uart::Controller::read_()
{
    return (uint8_t)USART1->RDR;
}

void uart::Controller::write_(uint8_t c)
{
    USART1->TDR = c;
}
