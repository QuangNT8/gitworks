#ifdef MCU_STM32F1
#include "controller.h"

extern foc::Controller focController;

void foc::Controller::initPWM_()
{
    /*TODO:*/
}

void foc::Controller::initADC_()
{
    /*TODO:*/
}

void foc::Controller::initEncoder_()
{
    /*TODO:*/
}

void foc::Controller::setDuties_(uint16_t pwmA, uint16_t pwmB, uint16_t pwmC)
{
    if (!motorEnabale_) pwmA = pwmB = pwmC = 0;
    /*TODO:*/
}

uint16_t foc::Controller::readEncoder_()
{
    /*TODO:*/
}

extern "C" void ADC_IRQHandler(void) //10kHz
{
    static uint16_t count = 0;
    focController.updateCurrents(0,0); //FIXME: ia, ib raw value from ADC
    count++;
    if (count > 9 )
    {
        focController.trigger(); //1kHz
        count = 0;
    }
}

#endif
