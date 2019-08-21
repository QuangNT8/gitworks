#include "stm32f0xx.h"
#include "RCservo.h"

#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal.h"


#define duty_valve_off  25
#define duty_valve_on1  10 //right
#define duty_valve_on2  38 //left




RCservo::Controller RCservo::CONTROLLER;


uint32_t RCcounter=0;
uint16_t enable_valveoff=GPIO_PIN_All;
uint16_t enable_valveon=0;
uint16_t duty_valveon=duty_valve_on1;


void RCservo::Controller::init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13
                              |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|      \
                          GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|    \
                          GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    RCservo::Controller::initTimer6();

}

void RCservo::Controller::initTimer6()
{
#if 1
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC = 5;
    TIM6->ARR = 480;
    TIM6->DIER = TIM_DIER_UIE;
    TIM6->CR1 = TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    NVIC_SetPriority(TIM6_DAC_IRQn,0);
#endif
}


void RCservo::Controller::loop(uint16_t valvest)
{
    switch (valvest)
    {
        case 0:
            valveSwitch(Valve1,OFF,left);
        break;
        case 1 :
            valveSwitch(Valve1,ON,right);
        break;
        case 2:
            valveSwitch(Valve2,ON,left);
        break;
        case 3:
            valveSwitch(Valve3,ON,left);
        break;
        case 4:
            valveSwitch(Valve4,ON,left);
        break;
        case 5:
            valveSwitch(Valve5,ON,left);
        break;
        case 6:
            valveSwitch(Valve6,ON,left);
        break;
        case 7:
            valveSwitch(Valve7,ON,left);
        break;
        case 8:
            valveSwitch(Valve8,ON,left);
        break;
        case 9:
            valveSwitch(Valve9,ON,left);
        break;
        case 10:
            valveSwitch(Valve10,ON,left);
        break;
        case 11:
            valveSwitch(Valve11,ON,left);
        break;
        case 12:
            valveSwitch(Valve12,ON,left);
        break;
    }
}

void RCservo::Controller::pulseUp()
{
   uint16_t i;
//   GPIOB->BSRR = 0xFFFF;
//   RCservo::CONTROLLER.RCcounter=0;
}

void pulselow()
{

}

void RCservo::Controller::pulseDown()
{

}
#if 1
extern "C" void TIM6_DAC_IRQHandler(void)
{
    if(TIM6->SR & TIM_SR_UIF)    // if UIF flag is set
    {
        TIM6->SR &= ~TIM_SR_UIF; // clear UIF flag
//        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        RCcounter++;
        if(RCcounter > duty_valve_off)
        {
            HAL_GPIO_WritePin(GPIOB,enable_valveoff,GPIO_PIN_RESET);
        }
        if(RCcounter > duty_valveon)
        {
            HAL_GPIO_WritePin(GPIOB,enable_valveon,GPIO_PIN_RESET);
        }
        if(RCcounter>330)
        {
            RCcounter=0;
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_All,GPIO_PIN_SET);
        }
    }
}
#endif

void RCservo::Controller::valveSwitch(ValvePindef pinvalve,ValveSwitch_st valvest,ValveDir_st dir)
{
    if(dir)duty_valveon=duty_valve_on1;
    else duty_valveon=duty_valve_on2;
    if(valvest == ON)
    {
        enable_valveoff = pinvalve^GPIO_PIN_All;
        enable_valveon = pinvalve;
    }
    else if(valvest == OFF)
    {
        enable_valveoff = GPIO_PIN_All;
        enable_valveon = 0;
    }
}

void RCservo::Controller::valve_toggle(uint16_t numbervalve)
{

}
