#include "stm32f0xx.h"
#include "pwm.h"
#include "command.h"
#include "uart/controller.h"

pwm::settup pwm::SETTUP;

void pwm::settup::init()
{
    initGpio_();
    TIM1->CCR1 = 600;
    TIM1->CCR2 = 200;
    TIM1->CCR3 = 200;
}

void pwm::settup::loop()
{
    cntTimer1 = TIM1->CCR1;
    if(countDelayPlot_++ > 30000)
    {
        uart::CONTROLLER.printfMessage("countTest = %d, %d", countTest_, cntTimer1);
        countDelayPlot_ = 0;
        countTest_++;
    }
}

void pwm::settup::initGpio_()
{
    /*
     * PWM-TIMER1-6CH
     * PA8  -> TIM1-CH1
     * PA9  -> TIM1-CH2
     * PA10 -> TIM1-CH3
     * PB13 -> TIM1-CH1N
     * PB14 -> TIM1-CH2N
     * PB15 -> TIM1-CH3N
     * PA15 -> Led
    */

    /*begin init GPIO*/
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIOA->MODER |= (GPIO_MODER_MODER15_0);         /*begin led*/
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_15);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR15);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR15);          /*end led*/

    GPIOA->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);                 /*begin pwm*/
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9  | GPIO_PUPDR_PUPDR10);
    GPIOA->AFR[1] = 0x00000222;//AF2

    GPIOB->MODER |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_14 | GPIO_OTYPER_OT_15);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13 | GPIO_PUPDR_PUPDR14  | GPIO_PUPDR_PUPDR15);                   /*end pwm*/
    GPIOB->AFR[1] = 0x22200000;//AF2
    /*end init GPIO*/

    /*begin init timer1*/
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->PSC = 0;
    TIM1->ARR = 1199;
    TIM1->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM1->CR1 = TIM_CR1_CEN;   // Enable timer

    /* set pwm1 mode (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    /* set pwm1 mode (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 3 */
    TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;

    /* Enable channel outputs */
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NE;
    TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM1->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3NE;

    /* output idle state HIGHT */
    TIM1->CR2 |= TIM_CR2_OIS1 | TIM_CR2_OIS1N;
    TIM1->CR2 |= TIM_CR2_OIS2 | TIM_CR2_OIS2N;
    TIM1->CR2 |= TIM_CR2_OIS3 | TIM_CR2_OIS3N;

    /* active LOW */
    TIM1->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
    TIM1->CCER |= TIM_CCER_CC2P | TIM_CCER_CC2NP;
    TIM1->CCER |= TIM_CCER_CC3P | TIM_CCER_CC3NP;

    /*dead time 0-255*/
    /*
     *DTG[7:5]=0xx => DT=DTG[7:0]x tdtg with tdtg=tDTS.
     *DTG[7:5]=10x => DT=(64+DTG[5:0])xtdtg with Tdtg=2xtDTS.
     *DTG[7:5]=110 => DT=(32+DTG[4:0])xtdtg with Tdtg=8xtDTS.
     *DTG[7:5]=111 => DT=(32+DTG[4:0])xtdtg with Tdtg=16xtDTS.
     *Example if TDTS=125 ns (8 MHz), dead-time possible values are:
     *0 to 15875 ns by 125 ns steps,
     *16 us to 31750 ns by 250 ns steps,
     *32 us to 63 us by 1 us steps,
     *64 us to 126 us by 2 us steps
    */
    TIM1->BDTR |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select center-aligned mode 1 (CMS = 01) */
    /* (8) Force update generation (UG = 1) */
    TIM1->BDTR |= TIM_BDTR_MOE; /* (6) */
    TIM1->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN; /* (7) */
    TIM1->EGR |= TIM_EGR_UG; /* (8) */
    /*end init timer1*/
}

