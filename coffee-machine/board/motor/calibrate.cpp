#include "define.h"
#include "controller.h"
#include "uart/controller.h"

void motor::Controller::calibrate_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 239;
    TIM2->ARR = 2000;	           // 10khz
    TIM2->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM2->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt from TIM2 (NVIC level)
    setDuties_(ALIGN_DUTY, 0, 0,0);
}

extern "C" void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF)    // if UIF flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
        static uint16_t count = 0;

        if (count++ == 300) //3s
        {
            /* read angle offset */
            CLEAR_SCS_AS5047;
            SPI1->DR = CMD_R_ANGLECOM;

            /* disable timer 3*/
            NVIC_DisableIRQ(TIM2_IRQn); // Disable interrupt from TIM2 (NVIC level)
            TIM2->CR1 &= (~TIM_CR1_CEN); //Disable timer
            count = 0;
        }
    }
}
