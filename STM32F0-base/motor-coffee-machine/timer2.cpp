#include "motor.h"
#include "uart/controller.h"
#include "RCservoLib/RCservo.h"
#include "stm32f0xx.h"


void step::Motor::initTimer2_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 4;
    TIM2->ARR = 480;//959;	           // 50khz <=> 0.02ms
    TIM2->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM2->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt from TIM2 (NVIC level)
//    NVIC_SetPriority(TIM2_IRQn,15);
}

int16_t cntTrigger = 0;
extern "C" void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF)    // if UIF flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
        //ADC1->CR |= ADC_CR_ADSTART;//50khz triger ADC
        cntTrigger++;
        if((cntTrigger % step::MOTOR.speed) == 0)//xxkhz
        {
            step::MOTOR.processCommand(step::MOTOR.RUN_PWM);
        }

        if(cntTrigger >= 5000)//50khz/5000 = 10hz (LED BLINK)
        {
            cntTrigger = 0;
            //uart::CONTROLLER.printfMessage("TIM2_IRQ Test = %d", EXTI->PR);
            LED_TOGGLE;
        }

        //RCservo::CONTROLLER.RCservo_controller();
    }
//    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
}
