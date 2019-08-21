#include "controller.h"
#include "command.h"

void motor::Controller::initGPIO_()
{
    /*********************************
     * PA0 - infrared sensor
     * ********************************
    */
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOAEN;    
    GPIOA   ->PUPDR     |= (GPIO_PUPDR_PUPDR0_0);
}

void motor::Controller::scanSensor_(int32_t position, uint8_t stage)
{
    static uint16_t cnt = 0;
    if(!((GPIOA->IDR & GPIO_IDR_0) >> 0))
    {
        cnt++;
    }
    else
    {
        cnt = 0;
    }
    if((cnt > 10))
    {
        setSpeed(200);
        startMotor();
        generator_.start(A_, V_, position << 10);
        zero = stage;
    }
}

void motor::Controller::setZero_()
{
    if(iOffsetCpl_ && setZero)
    {
        switch (zero)
        {
            case ZERO0:
                static uint16_t cnt1 = 0;
                scanSensor_(200, ZERO1);            //position sensor
                cnt1 ++;
                if (cnt1 > 1000)                    //position random
                {
                    zero = ZERO1;
                }
                break;
            case ZERO1:
                if(!moving())
                {
                    setSpeed(200);
                    startMotor();
                    generator_.start(A_, V_, -(4096<<10));
                    zero = ZERO2;
                }
                break;
            case ZERO2:
                scanSensor_(-10, ZERO3);
                break;
            case ZERO3:
                if(!moving())
                {
                    uart::CONTROLLER.printfMessage("Set Zero Complete!");
                    setSpeed(800);
                    zero = ZERO2;
                    setZero = false;
                    pMachine_ = 0;
                }
                break;
        }
    }
}
