#include "stm32f103xb.h"
#include "controller.h"

void uart::Controller::initUART_()
{
    // GPIOA Periph clock enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;


    //USART1 GPIO Configuration
    //PA9     ------> USART1_TX
    //PA10     ------> USART1_RX

    // Set alternate
    GPIOA->CRH |= (GPIO_CRH_CNF9_Msk);//output max speed
    GPIOA->CRH |= GPIO_CRH_MODE9_1;//10: Alternate function output Push-pull

//    GPIOA->CRH |= GPIO_CRH_CNF10_0;
//    GPIOA->CRH &= ~GPIO_CRH_MODE10_0;
//    GPIOA->CRH &= ~GPIO_CRH_MODE10_1;

    // USART1 clock enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 115200 Bd @ 48 MHz ???
    // USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
    // BRR[15:4] = USARTDIV[15:4]
    // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
    //USART1->BRR = (uint16_t)(0x01A0);

    USART1->BRR = (uint16_t)(0x0270);//72 115200
    //USART1->BRR = (uint16_t)(0x0230);//64 115200
    //USART1->BRR = (uint16_t)(0x1380);//48 9600
    //USART1->BRR = (uint16_t)(0x340);//8 9600
    //USART1->BRR = (uint16_t)(0x680);//16 9600
    //USART1->BRR = (uint16_t)(0xEA0);//36 9600
    //USART1->BRR = (uint16_t)(0x1380);//48 9600
    //USART1->BRR = (uint16_t)(0x01A0);//48 115200

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
