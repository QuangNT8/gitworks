#include "stm32f103xb.h"
#include "controller.h"

void uart2::Controller::initUART_()
{
    // GPIOA Periph clock enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;


    //USART2 GPIO Configuration
    //PA2     ------> USART2_TX
    //PA3     ------> USART2_RX

    // Set alternate
    GPIOA->CRL |= (GPIO_CRL_CNF2_Msk);//output max speed
    GPIOA->CRL |= GPIO_CRL_MODE2_1;//10: Alternate function output Push-pull

    GPIOA->CRL |= GPIO_CRL_CNF3_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE3_0;
    GPIOA->CRL &= ~GPIO_CRL_MODE3_1;

    // USART1 clock enable
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // 9600 Bd @ 36 MHz ???
    // USARTDIV = 36 MHz / (9600*16) = 234 = 0xEA
    // BRR[15:4] = USARTDIV[15:4]
    // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
    USART2->BRR = (uint16_t)(0x0EA0);//9600
    //USART2->BRR = (uint16_t)(0x0750);//19200

    // USART enable
    // Receiver enable
    // Transmitter enable
    USART2->CR1 = (uint32_t)(USART_CR1_UE |USART_CR1_RE |USART_CR1_TE);

    // Default value
    USART2->CR2 = (uint32_t)(0x00000000);
    USART2->CR3 = (uint32_t)(0x00000000);
}


bool uart2::Controller::rxReady_()
{
    return (USART2->SR & USART_SR_RXNE);
}

bool uart2::Controller::txReady_()
{
    return (USART2->SR & USART_SR_TXE) ;
}

uint8_t uart2::Controller::read_()
{
    return (uint8_t)USART2->DR;
}

void uart2::Controller::write_(uint8_t c)
{
    USART2->DR = c;
}
