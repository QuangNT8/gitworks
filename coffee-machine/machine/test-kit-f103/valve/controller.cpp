#include "controller.h"
#include "uart/controller.h"

valve::Controller valve::CONTROLLER;

void valve::Controller::loop()
{
    if (needUpdateTime_)
    {
        needUpdateTime_ = false;
        if (times_ > 0) times_--;

        if (times_ == 0)
        {
            if (state_ == OPEN)
            {
                state_ = DELAY;
                times_ = DELAY_TIME;
                moveMiddle_();
            }
            else if (state_ == DELAY)
            {
                state_ = CLOSE;
                //valveClosed_();
            }
        }
    }
}

void valve::Controller::moveLeft_()
{
    leftBits_  |= (1U << VALVE);
    midleBits_ &= (~(1U<< VALVE));
}

void valve::Controller::moveRight_()
{
    leftBits_  &= (~(1U << VALVE));
    midleBits_ &= (~(1U << VALVE));
}

void valve::Controller::moveMiddle_()
{
    midleBits_ |= (1U << VALVE);
    leftBits_  &= (~(1U << VALVE));
}

void valve::Controller::open(uint16_t time)
{
    if (time > 0)
    {
        times_ = time;
        state_ = OPEN;
        moveRight_();
        //valveOpened_();
    }
}


extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
        valve::CONTROLLER.updateSignals();
    }
}

void valve::Controller::updateSignals()
{
    if (pwmState_ == HIGH)
    {
        TIM3->ARR = LEFT_ARR;
        GPIOC->BSRR |= (1U << VALVE); //make PC15 to high
        pwmState_ = LEFT;
    }
    else if (pwmState_==LEFT)
    {
        TIM3->ARR = MIDDLE_ARR;
        GPIOC->BRR |= leftBits_;
        pwmState_ = MIDDLE;
    }
    else if (pwmState_ == MIDDLE)
    {
        TIM3->ARR = RIGHT_ARR;
        GPIOC->BRR |= midleBits_;
        pwmState_ = RIGHT;
    }
    else if (pwmState_ == RIGHT)
    {
        TIM3->ARR = HIGH_ARR;
        GPIOC->BRR |= (1U << VALVE); //make PC15 to low
        pwmState_ = HIGH;
        needUpdateTime_ = true;
    }
}

void valve::Controller::init()
{
    times_ = 0;
    state_ = CLOSE;

    midleBits_ = (1U << VALVE); // servo is reset at midle
    leftBits_ = 0;

    /* GPIO Ports Clock Enable */
    RCC     ->APB2ENR    |= RCC_APB2ENR_IOPCEN;
    GPIOC   ->CRH   |= (GPIO_CRH_MODE15_Msk);
    GPIOC   ->ODR   &= ~GPIO_ODR_ODR15;

    /*Timer 3*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 144;
    TIM3->ARR = 10000;         // 50Hz -> 72M/144/50 = 10000
    TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
}
