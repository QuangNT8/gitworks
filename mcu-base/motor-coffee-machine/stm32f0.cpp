#include "controller.h"
#include "stm32f0xx.h"
#include "hal/motor_adc.h"

void foc::Controller::initTimer3_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 39;
    TIM3->ARR = 9;
    TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
}

extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
        foc::Controller::speed++;
    }
}
