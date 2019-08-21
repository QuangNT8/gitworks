#include "uart/controller.h"
#include "command.h"

#include "stm32_timer/timer.h"
#include "Stm32_System_clock/osc.h"
#include "Stm32_GPIO/gpio.h"
#include "GPIOexpansion/GPIOexp.h"
#include "controller.h"
#include "stdio.h"
//#include "Stm32_USART/Usart.h"

#include "stm32f0xx_hal_gpio.h"
//#define init_pos 6
#define init_pos 16
#define open_pos 23

uint16_t servo_duty[16]={0},count_h[16]={0};
uint16_t motorcount=0;
uint16_t GPIO_NUM=GPIO_PIN_0;


void irq_timer(uint32_t event_type)
{
    for(motorcount=0;motorcount<16;motorcount++)
    {
        count_h[motorcount]++;
        if(servo_duty[motorcount]<init_pos) servo_duty[motorcount] = init_pos;
        if(servo_duty[motorcount]>open_pos) servo_duty[motorcount] = open_pos;
        if(count_h[motorcount] < servo_duty[motorcount])
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_NUM<<motorcount,GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_NUM<<motorcount,GPIO_PIN_RESET);
            if(count_h[motorcount]>198)
            {
               count_h[motorcount]=0;
            }
        }
    }
}

void valve_toggle(uint16_t numbervalve)
{
    if(servo_duty[numbervalve]<=init_pos)servo_duty[numbervalve]=open_pos;
    else if(servo_duty[numbervalve]>=open_pos) servo_duty[numbervalve]=init_pos;
}

void RCservor_controller()
{
    uint16_t i;
    if(plot::CONTROLLER.valveisclicked[0]==1)
    {
        valve_toggle(0);
        plot::CONTROLLER.valveisclicked[0]=0;
    }
    if(plot::CONTROLLER.valveisclicked[1]==1)
    {
        valve_toggle(1);
        plot::CONTROLLER.valveisclicked[1]=0;
    }
    if(plot::CONTROLLER.valveisclicked[2]==1)
    {
        valve_toggle(2);
        plot::CONTROLLER.valveisclicked[2]=0;
    }
    if(plot::CONTROLLER.valveisclicked[3]==1)
    {
        valve_toggle(3);
        plot::CONTROLLER.valveisclicked[3]=0;
    }
    if(plot::CONTROLLER.valveisclicked[8]==1)
    {
        valve_toggle(8);
        plot::CONTROLLER.valveisclicked[8]=0;
    }
    if(plot::CONTROLLER.valveisclicked[9]==1)
    {
        valve_toggle(9);
        plot::CONTROLLER.valveisclicked[9]=0;
    }
    if(plot::CONTROLLER.valveisclicked[10]==1)
    {
        valve_toggle(10);
        plot::CONTROLLER.valveisclicked[10]=0;
    }
    if(plot::CONTROLLER.valveisclicked[11]==1)
    {
        valve_toggle(11);
        plot::CONTROLLER.valveisclicked[11]=0;
    }
    if(plot::CONTROLLER.valveisclicked[12]==1)
    {
        valve_toggle(12);
        plot::CONTROLLER.valveisclicked[12]=0;
    }
    if(plot::CONTROLLER.valveisclicked[13]==1)
    {
        valve_toggle(13);
        plot::CONTROLLER.valveisclicked[13]=0;
    }
    if(plot::CONTROLLER.valveisclicked[14]==1)
    {
        valve_toggle(14);
        plot::CONTROLLER.valveisclicked[14]=0;
    }
    if(plot::CONTROLLER.valveisclicked[15]==1)
    {
        valve_toggle(15);
        plot::CONTROLLER.valveisclicked[15]=0;
    }
}
int main()
{
//        blink::CONTROLLER.init();
//    osc::CONTROLLER.HAL_Init();
    timer::CONTROLLER.init_IT(irq_timer);
    GPIO::CONTROLLER.init();
//    GPIOexp::CONTROLLER.init();
    uart::CONTROLLER.init();
    plot::CONTROLLER.init();
//    servo_duty[0]=100;
//    servo_duty[1]=16;
//    servo_duty[2]=15;
//    servo_duty[3]=15;
//    servo_duty[8]=15;
//    servo_duty[9]=15;
//    servo_duty[10]=15;
//    servo_duty[11]=15;
//    servo_duty[12]=15;
//    servo_duty[13]=15;
//    servo_duty[14]=15;
//    servo_duty[15]=200;
//    GPIO_NUM<<=1;
//    USART::CONTROLLER.init();
    while (true)
    {
//        timer::CONTROLLER.loop();
//        blink::CONTROLLER.loop();
        uart::CONTROLLER.loop();
        plot::CONTROLLER.loop();
//        servo_duty[0]=plot::CONTROLLER.MotorArray[0];
//        servo_duty[1]=plot::CONTROLLER.MotorArray[1];
//        servo_duty[2]=plot::CONTROLLER.MotorArray[2];
        RCservor_controller();
//        uart::CONTROLLER.printfMessage("test");
//        GPIO::CONTROLLER.loop();
//        USART::CONTROLLER.loop();
    }
//    return 0;
}
