#include "controller.h"
#include "command.h"

void kit::Controller::initGPIO_()
{
    /*********************************
     * PC14 - Pump
     * ********************************
    */
    RCC     ->APB2ENR    |= RCC_APB2ENR_IOPCEN;
    GPIOC   ->CRH   |= (GPIO_CRH_MODE14_Msk);
    GPIOC   ->ODR   &= ~GPIO_ODR_ODR14;
    GPIOC   ->CRH   |= (GPIO_CRH_MODE13_Msk);
    GPIOC   ->ODR   &= ~GPIO_ODR_ODR13;
}

void kit::Controller::togglePump()
{
    GPIOC->ODR ^= GPIO_ODR_ODR14;
    GPIOC->ODR ^= GPIO_ODR_ODR13;
}

void kit::Controller::closePump()
{
    GPIOC->BRR |= GPIO_BRR_BR14;
    GPIOC->BRR |= GPIO_BRR_BR13;
}

void kit::Controller::openPump()
{
    GPIOC->BSRR |= GPIO_BSRR_BS14;
    GPIOC->BSRR |= GPIO_BSRR_BS13;
}
