#include "controller.h"
#include "uart/controller.h"

void motor::Controller::initGPIO_()
{
    /*********************************
     * PA0 - infrared sensor
     * ********************************
    */
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOAEN;
    GPIOA   ->PUPDR     |= (GPIO_PUPDR_PUPDR0_0);
}
