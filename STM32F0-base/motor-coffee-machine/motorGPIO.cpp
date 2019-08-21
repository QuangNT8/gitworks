#include "motor.h"

void step::Motor::initGPIO_()
{
    /*********************************
     * PC9 - LED TEST
     * PC2 - Cong tac hanh trinh
     * ********************************
    */
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOCEN;

    GPIOC   ->MODER     |= (GPIO_MODER_MODER9_0);
    GPIOC   ->OTYPER    &= ~(GPIO_OTYPER_OT_9);
    GPIOC   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR9);
    GPIOC   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR9);

    cntPC2 = 0;
    GPIOC   ->PUPDR     |= (GPIO_PUPDR_PUPDR2_0);
}

void step::Motor::loopPC2_()
{
    static uint32_t temp = 0;
    if(temp < 10000)
    {
        if((GPIOC->IDR & GPIO_IDR_2) >> 2)
        {
            cntPC2++;
        }
        else
        {
            cntPC2 = 0;
        }
        if((cntPC2 > 40))
        {
            //uart::CONTROLLER.printfMessage("test OK = %d", temp);
            temp++;
        }
    }
}
