#include "controller.h"

//#define LEFT_ARR    500 //1ms , FIXME:
//#define MIDDLE_ARR  250 //1.5ms, FIXME:
//#define RIGHT_ARR   250 //2ms, FIXME:
//#define HIGH_ARR    (10000 - LEFT_ARR - MIDDLE_ARR - RIGHT_ARR)

#define LEFT_ARR    250//new mr Hung
#define MIDDLE_ARR  420
#define RIGHT_ARR   500
#define HIGH_ARR    (10000 - LEFT_ARR - MIDDLE_ARR - RIGHT_ARR)

#define ALL_VALVES  0xFFFFU

#define MIN_VALVE_INDEX  0
#define MAX_VALVE_INDEX  15

valve::Controller valve::CONTROLLER;

void valve::Controller::loop()
{
    if (opening_)
    {
        if (openTimeCount_ >= openTime_)
        {
            opening_ = false;
            midleBits_ = ALL_VALVES;
            leftBits_  &= (~(1U<<currentIndex_));
        }
    }
    if(delaying_)
    {
        if(delayTimeCount_ >= delayTime_)
        {
            delaying_ = false;
        }
    }
}

void valve::Controller::moveLeft_(uint8_t index)
{
    leftBits_  |= (1U << index);
    midleBits_ &= (~(1U<< index));
}

void valve::Controller::moveRight_(uint8_t index)
{
    leftBits_  &= (~(1U << index));
    midleBits_ &= (~(1U << index));
}

void valve::Controller::moveMiddle_(uint8_t index)
{
    midleBits_ |= (1U << index);
    leftBits_  &= (~(1U << index));
}

void valve::Controller::open(uint8_t index, uint16_t time)
{
    if (opening_) return;
    if (index < MIN_VALVE_INDEX || index > MAX_VALVE_INDEX) return;
    if (time > 0)
    {
        moveRight_(index);
        openTimeCount_ = 0;
        currentIndex_ = index;
        openTime_ = time;
        opening_ = true;
    }
}

void valve::Controller::delay(uint16_t time)
{
    if(delaying_) return;
    delayTimeCount_ = 0;
    delayTime_ = time;
    delaying_ = true;
}

extern "C" void TIM6_DAC_IRQHandler(void)
{
    if(TIM6->SR & TIM_SR_UIF)    // if UIF flag is set
    {
        TIM6->SR &= ~TIM_SR_UIF; // clear UIF flag
        valve::CONTROLLER.updateSignals();
    }
}

void valve::Controller::updateSignals()
{
    if (pwmState_ == HIGH)
    {
        TIM6->ARR = LEFT_ARR;
        GPIOB->BSRR = ALL_VALVES; //make all ports to high
        pwmState_ = LEFT;
    }
    else if (pwmState_==LEFT)
    {
        TIM6->ARR = MIDDLE_ARR;
        GPIOB->BRR = leftBits_;
        pwmState_ = MIDDLE;
    }
    else if (pwmState_ == MIDDLE)
    {
        TIM6->ARR = RIGHT_ARR;
        GPIOB->BRR = midleBits_;
        pwmState_ = RIGHT;
    }
    else if (pwmState_ == RIGHT)
    {
        TIM6->ARR = HIGH_ARR;
        GPIOB->BRR = ALL_VALVES; //make all port to low
        pwmState_ = HIGH;
        openTimeCount_++;
        delayTimeCount_++;
    }
}

void valve::Controller::init()
{
    midleBits_ = ALL_VALVES; //all servo is reset at midle
    leftBits_ = 0;

    /* GPIO Ports Clock Enable */
    RCC->AHBENR|=RCC_AHBENR_GPIOBEN;

    //*Configure GPIO pin Output Level */
    GPIOB->MODER |= (uint32_t)(GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0|GPIO_MODER_MODER2_0|GPIO_MODER_MODER3_0|\
                               GPIO_MODER_MODER4_0|GPIO_MODER_MODER5_0|GPIO_MODER_MODER6_0|GPIO_MODER_MODER7_0|\
                               GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0|GPIO_MODER_MODER10_0|GPIO_MODER_MODER11_0|\
                               GPIO_MODER_MODER12_0|GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0|GPIO_MODER_MODER15_0);

    // Ensure push pull mode selected--default
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_0|GPIO_OTYPER_OT_1|GPIO_OTYPER_OT_2|GPIO_OTYPER_OT_3|\
                       GPIO_OTYPER_OT_4|GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_6|GPIO_OTYPER_OT_7|\
                       GPIO_OTYPER_OT_8|GPIO_OTYPER_OT_9|GPIO_OTYPER_OT_10|GPIO_OTYPER_OT_11|\
                       GPIO_OTYPER_OT_12|GPIO_OTYPER_OT_13|GPIO_OTYPER_OT_14|GPIO_OTYPER_OT_15);
    //Ensure maximum speed setting (even though it is unnecessary)
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0|GPIO_OSPEEDER_OSPEEDR1|GPIO_OSPEEDER_OSPEEDR2|GPIO_OSPEEDER_OSPEEDR3|\
                       GPIO_OSPEEDER_OSPEEDR4|GPIO_OSPEEDER_OSPEEDR5|GPIO_OSPEEDER_OSPEEDR6|GPIO_OSPEEDER_OSPEEDR7|\
                       GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9|GPIO_OSPEEDER_OSPEEDR10|GPIO_OSPEEDER_OSPEEDR11|\
                       GPIO_OSPEEDER_OSPEEDR12|GPIO_OSPEEDER_OSPEEDR13|GPIO_OSPEEDER_OSPEEDR14|GPIO_OSPEEDER_OSPEEDR15);

    //Ensure all pull up pull down resistors are disabled
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0|GPIO_PUPDR_PUPDR1|GPIO_PUPDR_PUPDR2|GPIO_PUPDR_PUPDR3|\
                      GPIO_PUPDR_PUPDR4|GPIO_PUPDR_PUPDR5|GPIO_PUPDR_PUPDR6|GPIO_PUPDR_PUPDR7|\
                      GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9|GPIO_PUPDR_PUPDR10|GPIO_PUPDR_PUPDR11|\
                      GPIO_PUPDR_PUPDR12|GPIO_PUPDR_PUPDR13|GPIO_PUPDR_PUPDR14|GPIO_PUPDR_PUPDR15);

    RCC->AHBENR|=RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (uint32_t)(GPIO_MODER_MODER8_0);
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8);
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8);

    /*Timer 6*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 95; /*  */
    TIM6->ARR = 10000; /* 50Hz -> 48M/96/50 = 10000 */
    TIM6->DIER = TIM_DIER_UIE;
    TIM6->CR1 = TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    NVIC_SetPriority(TIM6_DAC_IRQn,0);
}
