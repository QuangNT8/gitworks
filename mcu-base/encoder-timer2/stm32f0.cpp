#include "stm32f0xx.h"
#include "encoder.h"
#include "command.h"
#include "uart/controller.h"

uint32_t cntTimer2;

encoder::settup encoder::SETTUP;

void encoder::settup::init()
{
    initGpio_();
}

void encoder::settup::loop()
{
    if(countDelayPlot_++ > 30000)
    {
        cntEncoder_ = TIM2->CNT;
        uart::CONTROLLER.printfMessage("TIM2->CNT = %d, countTest = %d",cntEncoder_, countTest_);
        countDelayPlot_ = 0;
        countTest_ = cntTimer2;
    }
}

void encoder::settup::initGpio_()
{
    /*
     * ENCODER-TIMER2
     * PA0 -> Encoder ch1
     * PA1 -> Encoder ch2
     * PA15 -> Led
    */

    /*begin init GPIO*/
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1);
    GPIOA->AFR[0] |= 34;
    /*end init GPIO*/

    /*begin init timer2*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 0;
    TIM2->ARR = 2399;
    TIM2->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)

    /* (1) Configure TI1FP1 on TI1 (CC1S = 01), configure TI1FP2 on TI2 (CC2S = 01) */
    /* (2) Configure TI1FP1 and TI1FP2 non inverted (CC1P = CC2P = 0, reset value) */
    /* (3) Configure both inputs are active on both rising and falling edges (SMS = 011) */
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0; /* (1)*/
    TIM2->CCER &= (uint16_t)(~(TIM_CCER_CC1P | TIM_CCER_CC2P)); /* (2) */
    TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; /* (3) */
    TIM2->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt from TIM2 (NVIC level)
    /*end init timer3*/
}

extern "C" void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
        cntTimer2++;
    }
}
