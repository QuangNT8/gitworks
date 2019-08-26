#include "stm32f042x6.h"
#include "controller.h"

void uart::Controller::initUART_()
{
    // GPIOB Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // PA2 and PA3 Alternate function mode
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);

    // Set alternate functions AF1 for PA2 and PA3
    GPIOA->AFR[0] |= 0x00001100;

    // USART2 clock enable
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // 115200 Bd @ 48 MHz ???
    // USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
    // BRR[15:4] = USARTDIV[15:4]
    // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
    USART2->BRR = (uint16_t)(0x01A0);

    // USART enable
    // Receiver enable
    // Transmitter enable
    USART2->CR1 = (uint32_t)(USART_CR1_UE |USART_CR1_RE |USART_CR1_TE);

    // Default value
    USART2->CR2 = (uint32_t)(0x00000000);
    USART2->CR3 = (uint32_t)(0x00000000);
}


bool uart::Controller::rxReady_()
{
    return (USART2->ISR & USART_ISR_RXNE);
}

bool uart::Controller::txReady_()
{
    return (USART2->ISR & USART_ISR_TXE) ;
}

uint8_t uart::Controller::read_()
{
    return (uint8_t)USART2->RDR;
}

void uart::Controller::write_(uint8_t c)
{
    USART2->TDR = c;
}
