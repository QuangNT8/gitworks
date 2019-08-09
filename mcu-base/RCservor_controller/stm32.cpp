#include "controller.h"
#include "stm32f0xx.h"
#define BSRR_VAL        0x0300

blink::Controller blink::CONTROLLER;

void blink::Controller::init()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0) ;
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9) ;
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9);

}


bool onOffbool = true;
void blink::Controller::loop()
{
    static uint32_t counterLed = 0;
    if(counterLed++ > (1 << 18))
    {
        GPIOC->ODR ^= BSRR_VAL;
//        onOffbool = !onOffbool;
//        if(onOffbool)
//        {
//            ledOn_();
//        }
//        else
//        {
//            ledOff_();
//        }
        counterLed = 0;
    }
}

void blink::Controller::ledOn_()
{
    GPIOC->BSRR = BSRR_VAL;
}

void blink::Controller::ledOff_()
{
    GPIOC->BRR = BSRR_VAL;
}


//void delay (int a)
//{
//    volatile int i,j;
//    for (i=0 ; i < a ; i++){j++;}
//    return;
//}
