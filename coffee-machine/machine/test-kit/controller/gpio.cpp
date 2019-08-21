#include "controller.h"
#include "command.h"
#include "controller/controller.h"

void testkit::Controller::initGPIO_()
{
    /*********************************
     * PC4 - Pump
     * ********************************
    */
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOCEN;
    GPIOC   ->MODER     |= GPIO_MODER_MODER4_0;/*01: General purpose output mode*/
    GPIOC   ->OTYPER    &= ~GPIO_OTYPER_OT_4;/* push-pull*/
    GPIOC   ->OSPEEDR   |= GPIO_OSPEEDER_OSPEEDR4;/* High speed*/
    GPIOC   ->PUPDR     |= GPIO_PUPDR_PUPDR4_0;/*pull-up*/
}

void testkit::Controller::togglePump()
{
    GPIOC->ODR ^= 0x10;
}

void testkit::Controller::closePump()
{
    GPIOC->BRR = 0x10;
}

void testkit::Controller::openPump()
{
    GPIOC->BSRR = 0x10;
}
