#ifdef NRF52
#include "controller.h"
#include "nrf.h"

extern foc::Controller focController;

void foc::Controller::initPWM_()
{
    /*TODO:*/
}

void foc::Controller::initADC_()
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0){}

    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;  // Set the timer in Counter Mode
    NRF_TIMER2->TASKS_CLEAR = 1;               // clear the task first to be usable for later
    NRF_TIMER2->PRESCALER = 6;                             //Prescaler: 16M/2^6 = 250000
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;		 //Set counter to 16 bit resolution
    NRF_TIMER2->CC[0] = 2500;                             //1ms ~ 1kHz
    NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
    //NVIC_ClearPendingIRQ(TIMER2_IRQn);
    NVIC_EnableIRQ(TIMER2_IRQn);
    NRF_TIMER2->TASKS_START = 1;               // Start TIMER2

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

extern "C" void TIMER2_IRQHandler(void) //1Khz: for test only
{
    if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
    {
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;
        NRF_TIMER2->SHORTS = 1 << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
        //NRF_TIMER2->TASKS_STOP = 1;
        foc::CONTROLLER.trigger(); //1kHz
    }
}

#endif
