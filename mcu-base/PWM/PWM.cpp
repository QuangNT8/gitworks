
#include "stm32f0xx.h"
#include "PWM.h"


/**@brief Timer control block. */


PWM::Controller PWM::CONTROLLER;



void PWM::Controller::init()
{
    /*********************************
     * PA8 - IN1 - A+ (PWM TIMER1-CH1)
     * PA9 - IN2 - A- IN-OUT
     * PB5 - IN3 - B+ (PWM TIMER3-CH2)
     * PB4 - IN4 - B- IN-OUT
     * ********************************
    */
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOAEN;
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOBEN;

    GPIOA   ->MODER     |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);                 /*begin pwm*/
    GPIOA   ->OTYPER    &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);
    GPIOA   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOA   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);                    /*end pwm*/
    GPIOA   ->AFR[0]    |= 0x11000000;  //AF1 assign to TIM3

    /*begin init timer3*/
    RCC     ->APB1ENR   |= RCC_APB1ENR_TIM3EN;
    TIM3    ->PSC       = 0;
    TIM3    ->ARR       = 1199;
    TIM3    ->DIER      = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3    ->CR1       = TIM_CR1_CEN;  // Enable timer

    /* set pwm mode 1 (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM3    ->CCMR1     |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    /* Enable channel outputs */
    TIM3    ->CCER      |= TIM_CCER_CC1E;
    TIM3    ->CCER      |= TIM_CCER_CC2E;

    /* output idle state HIGHT */
    TIM3    ->CR2       |= TIM_CR2_OIS1;
    TIM3    ->CR2       |= TIM_CR2_OIS2;

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
    TIM3    ->BDTR      |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select Edge-aligned (CMS = 00) */
    /* (8) Force update generation (UG = 1) */
    TIM3    ->BDTR      |= TIM_BDTR_MOE; /* (6) */
    TIM3    ->CR1       |= TIM_CR1_CEN; /* (7) */
    TIM3    ->EGR       |= TIM_EGR_UG; /* (8) */
    /*end init timer3*/


    GPIOA   ->MODER     |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1);                 /*begin pwm*/
    GPIOA   ->OTYPER    &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10);
    GPIOA   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
    GPIOA   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR10);                    /*end pwm*/
    GPIOA   ->AFR[1]    |= 0x00000222;//AF2

    /*begin init timer1*/
    RCC     ->APB2ENR   |= RCC_APB2ENR_TIM1EN;
    TIM1    ->PSC       = 0;
    TIM1    ->ARR       = 1199;
    TIM1    ->DIER      = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM1    ->CR1       = TIM_CR1_CEN;  // Enable timer

    /* set pwm mode 1 (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM1    ->CCMR1     |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | 	\
                           TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | 	\
                           TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;


    /*begin test*/
    TIM1    ->CCMR2     |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 |\
                           TIM_CCMR2_OC3PE;/* set pwm mode 2 (OCxM = 111) and enable preload (OCxPE = 1) channel 3 */
    TIM1    ->CCMR2     &= ~TIM_CCMR2_OC3FE;/*fast mode disable*/
    TIM1    ->CCR3      = 10;/*pulse*/
    TIM1    ->CCER      &= ~TIM_CCER_CC3E;/*CH polarity*/
    TIM1    ->CR2       &= ~TIM_CR2_OIS3;/*CH idele state LOW*/
    TIM1    ->CR2       |= TIM_CR2_MMS_2|TIM_CR2_MMS_1;/*110: Compare - OC3REF signal is used as trigger output (TRGO)*/
    /*end test*/

    /* Enable channel outputs */
    TIM1    ->CCER      |= TIM_CCER_CC1E;
    TIM1    ->CCER      |= TIM_CCER_CC2E;
    TIM1    ->CCER      |= TIM_CCER_CC3E;

    /* output idle state HIGHT */
    TIM1    ->CR2       |= TIM_CR2_OIS1;
    TIM1    ->CR2       |= TIM_CR2_OIS2;

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
    TIM1    ->BDTR      |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select Edge-aligned (CMS = 00) */
    /* (8) Force update generation (UG = 1) */
    TIM1    ->BDTR      |= TIM_BDTR_MOE; /* (6) */
    TIM1    ->CR1       |= TIM_CR1_CEN; /* (7) */
    TIM1    ->EGR       |= TIM_EGR_UG; /* (8) */
    /*end init timer1*/
}

void PWM::Controller::init2()
{
    /*********************************
     * PA8 - IN1 - A+ (PWM TIMER1-CH1)
     * PA9 - IN2 - A- IN-OUT
     * PB5 - IN3 - B+ (PWM TIMER3-CH2)
     * PB4 - IN4 - B- IN-OUT
     * ********************************
    */
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOAEN;
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOBEN;

    GPIOB   ->MODER     |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1);                 /*begin pwm*/
    GPIOB   ->OTYPER    &= ~(GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5);
    GPIOB   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5);
    GPIOB   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);                    /*end pwm*/
    GPIOB   ->AFR[0]    |= 0x00110000;//AF1

    /*begin init timer3*/
    RCC     ->APB1ENR   |= RCC_APB1ENR_TIM3EN;
    TIM3    ->PSC       = 0;
    TIM3    ->ARR       = 1199;
    TIM3    ->DIER      = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3    ->CR1       = TIM_CR1_CEN;  // Enable timer

    /* set pwm mode 1 (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM3    ->CCMR1     |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    /* Enable channel outputs */
    TIM3    ->CCER      |= TIM_CCER_CC1E;
    TIM3    ->CCER      |= TIM_CCER_CC2E;

    /* output idle state HIGHT */
    TIM3    ->CR2       |= TIM_CR2_OIS1;
    TIM3    ->CR2       |= TIM_CR2_OIS2;

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
    TIM3    ->BDTR      |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select Edge-aligned (CMS = 00) */
    /* (8) Force update generation (UG = 1) */
    TIM3    ->BDTR      |= TIM_BDTR_MOE; /* (6) */
    TIM3    ->CR1       |= TIM_CR1_CEN; /* (7) */
    TIM3    ->EGR       |= TIM_EGR_UG; /* (8) */
    /*end init timer3*/


    GPIOA   ->MODER     |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1);                 /*begin pwm*/
    GPIOA   ->OTYPER    &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
    GPIOA   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
    GPIOA   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);                    /*end pwm*/
    GPIOA   ->AFR[1]    |= 0x00000022;//AF2

    /*begin init timer1*/
    RCC     ->APB2ENR   |= RCC_APB2ENR_TIM1EN;
    TIM1    ->PSC       = 0;
    TIM1    ->ARR       = 1199;
    TIM1    ->DIER      = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM1    ->CR1       = TIM_CR1_CEN;  // Enable timer

    /* set pwm mode 1 (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM1    ->CCMR1     |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    /*begin test*/
    TIM1    ->CCMR2     |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4PE;/* set pwm mode 2 (OCxM = 111) and enable preload (OCxPE = 1) channel 4 */
    TIM1    ->CCMR2     &= ~TIM_CCMR2_OC4FE;/*fast mode disable*/
    TIM1    ->CCR4      = 10;/*pulse*/
    TIM1    ->CCER      &= ~TIM_CCER_CC4E;/*CH polarity*/
    TIM1    ->CR2       &= ~TIM_CR2_OIS4;/*CH idele state LOW*/
    TIM1    ->CR2       |= TIM_CR2_MMS;/*111: Compare - OC4REF signal is used as trigger output (TRGO)*/
    /*end test*/

    /* Enable channel outputs */
    TIM1    ->CCER      |= TIM_CCER_CC1E;
    TIM1    ->CCER      |= TIM_CCER_CC2E;

    /* output idle state HIGHT */
    TIM1    ->CR2       |= TIM_CR2_OIS1;
    TIM1    ->CR2       |= TIM_CR2_OIS2;

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
    TIM1    ->BDTR      |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select Edge-aligned (CMS = 00) */
    /* (8) Force update generation (UG = 1) */
    TIM1    ->BDTR      |= TIM_BDTR_MOE; /* (6) */
    TIM1    ->CR1       |= TIM_CR1_CEN; /* (7) */
    TIM1    ->EGR       |= TIM_EGR_UG; /* (8) */
    /*end init timer1*/
}

void PWM::Controller::loop()
{

}

void PWM::Controller::setDuties(uint16_t SW1, uint16_t SW2, uint16_t SW3, uint16_t SW4,uint16_t SW5)
{
    TIM1->CCR1 = SW3;
    TIM1->CCR2 = SW2;
    TIM1->CCR3 = SW1;
    TIM3->CCR2 = SW4;
    TIM3->CCR1 = SW5;
}
