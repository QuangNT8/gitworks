#include "controller.h"
#include "uart/controller.h"
#include "stm32f0xx.h"
#include "command.h"

void foc::Controller::initPWM_()
{
    /*TODO:*/
}

void foc::Controller::initADC_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 23999;	        // Set prescaler to 24 000 (PSC + 1) 48000000/24000 = 2000
    TIM3->ARR = 2;	          // Auto reload value 20 ~ 100hZ
    TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
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

#include <stdlib.h>
extern "C" void TIM3_IRQHandler(void)
{
    static int count = 0;
    if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag

        static uint16_t theta = 0;
        theta+=2;
        if (theta >= PULSE_2_PI) theta=0;

        int16_t sinTheta_, cosTheta_;

        if (theta < PULSE_PI_2)
        {
            sinTheta_ = SINCOS[theta];
            cosTheta_ = SINCOS[PULSE_PI_2 - theta];
        }
        else if (theta < PULSE_PI)
        {
            sinTheta_ = SINCOS[PULSE_PI - theta];
            cosTheta_ = -SINCOS[theta - PULSE_PI_2];
        }
        else if (theta < PULSE_3_PI_2)
        {
            sinTheta_ = -SINCOS[theta - PULSE_PI];
            cosTheta_ = -SINCOS[PULSE_3_PI_2 - theta];
        }
        else if (theta < PULSE_2_PI)
        {
            sinTheta_ = -SINCOS[PULSE_2_PI - theta];
            cosTheta_ = SINCOS[theta -PULSE_3_PI_2];
        }
        int8_t tmp = rand()%100;
        uint16_t s = plot::CONTROLLER.plot_sine(sinTheta_+tmp);
        plot::CONTROLLER.plot_cosine(cosTheta_+tmp, s);

        if (count++>9)
        {
            foc::CONTROLLER.trigger();
            count=0;
        }

        //foc::CONTROLLER.trigger();
    }
}
