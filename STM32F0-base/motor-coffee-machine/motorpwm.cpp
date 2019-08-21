/*
 * PWM = 40Khz
 * Resolution duty = 1200
 */
#include "motor.h"

void step::Motor::initPWM_()
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

void step::Motor::setDuties_(uint16_t pwmAh, uint16_t pwmAl, uint16_t pwmBh, uint16_t pwmBl)
{
    TIM1->CCR1 = pwmAh;
    TIM1->CCR2 = pwmAl;
    TIM3->CCR2 = pwmBh;
    TIM3->CCR1 = pwmBl;
}

void step::Motor::runPWM_(bool dir_)
{
    command_ = NONE;
    static uint16_t theta = 0;
    static uint8_t quadrant = 1;
    static uint32_t round = 0;
    if(motorEnabled_)
    {
        if (theta >= PULSE_PI_2)
        {
            theta = 0;
            quadrant++;
            if(quadrant >= 5)
            {
                round++;
                plot::CONTROLLER.update_round(round/50);
                quadrant = 1;
                if(round >= 20*50)                                 //1.8 degrees <=> 360/1.8 = 200 step
                                                                    //1 vong co <=> 200/4 = 50
                                                                    //round = x*50 = x vong co
                {
                    round= 0;
                    setDuties_(1199,1199,1199,1199);
                    motorEnabled_ = false;
                }
            }
        }
        sinTheta_ = SINCOS[theta] ;
        cosTheta_ = SINCOS[PULSE_PI_2 - theta];

        if(dir_)
        {
            switch (quadrant)
            {                                                           //  A+      A-      B+      B-
            case 1: setDuties_(cosTheta_, 0, sinTheta_, 0); break;      //  pwmA    0       pwmB    0       quadrant1
            case 2: setDuties_(0, sinTheta_, cosTheta_, 0); break;      //  0       pwmA    pwmB    0       quadrant2
            case 3: setDuties_(0, cosTheta_, 0, sinTheta_); break;      //  0       pwmA    0       pwmB    quadrant3
            case 4: setDuties_(sinTheta_, 0, 0, cosTheta_); break;      //  pwmA    0       0       pwmB    quadrant4
            }
        }
        else
        {
            switch (quadrant)
            {                                                           //  A+      A-      B+      B-
            case 1: setDuties_(cosTheta_, 0, 0, sinTheta_); break;      //  pwmA    0       0       pwmB    quadrant1
            case 2: setDuties_(0, sinTheta_, 0, cosTheta_); break;      //  0       pwmA    0       pwmB    quadrant2
            case 3: setDuties_(0, cosTheta_, sinTheta_, 0); break;      //  0       pwmA    pwmB    0       quadrant3
            case 4: setDuties_(sinTheta_, 0, cosTheta_, 0); break;      //  pwmA    0       pwmB    0       quadrant4
            }
        }
        theta++;
    }
    else
    {
        setDuties_(1199,1199,1199,1199);
        round = 0;
    }
}
