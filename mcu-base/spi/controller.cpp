/*
 * Controller.cpp
 *
 *  Created on: Oct 4, 2018
 *      Author: vanquang
 */

#include "controller.h"
#include "stm32f0xx.h"

spi::Controller spi::CONTROLLER;

void spi::Controller::loop()
{

}

void spi::Controller::init()
{
    initSpi_();
    DRV8323_setup();
}

int16_t spi::Controller::DRV8323_read(int8_t regAdr)
{
    int16_t temp = 0;
    int16_t controlword = 0x8000 | ((regAdr & 0x7) << 11);
    GPIOA->BRR = GPIO_BRR_BR_4;//reset SCS (PA4)
    for(int i = 0; i < 16; i++)
    {
        if((controlword & 0x8000)==0x8000)
        {
            GPIOA->BSRR = GPIO_BSRR_BS_7;//set SDI drv8323 (PA7)
        } else {
            GPIOA->BRR = GPIO_BRR_BR_7;//reset SDI drv8323 (PA7)
        }

        GPIOA->BSRR = GPIO_BSRR_BS_5;//set CLK (PA5)
        controlword <<= 1;

        /*TODO: delay*/
        for(int32_t delay2, delay1 = 0; delay1 < 3; delay1++)
        {
            delay2++;
        }

        GPIOA->BRR = GPIO_BRR_BR_5;//reset CLK (PA5)

        if(i>=5)
        {
            temp <<= 1;
            if(GPIOA->IDR == GPIO_IDR_6)//read SDO drv8323 (PA6)
            {
                temp |= 0x1;
            }
        }
    }

    /*TODO: delay*/
    for(int32_t delay2, delay1 = 0; delay1 < 3; delay1++)
    {
        delay2++;
    }

    GPIOA->BSRR = GPIO_BSRR_BS_4;//set SCS (PA4)

    /*TODO: delay*/
    for(int32_t delay2, delay1 = 0; delay1<30; delay1++)
    {
        delay2++;
    }
    return (0x7ff&temp);
}

void spi::Controller::DRV8323_write(int8_t addr,int16_t dataT)
{
    /*TODO*/
    int16_t controlword = ((addr & 0x7) << 11) | (dataT & 0x7ff);
    GPIOA->BRR = GPIO_BRR_BR_4; //reset SCS (PA4)
    for(int i = 0; i < 16; i++)
    {
        if((controlword & 0x8000) == 0x8000)
        {
            GPIOA->BSRR = GPIO_BSRR_BS_7;//set SDI drv8323 (PA7)
        } else {
            GPIOA->BRR = GPIO_BRR_BR_7;//reset SDI drv8323 (PA7)
        }

        /*TODO: delay*/
        for(int32_t delay2, delay1 = 0; delay1 < 30; delay1++)
        {
            delay2++;
        }

        GPIOA->BSRR = GPIO_BSRR_BS_5;//set CLK (PA5)
        controlword <<= 1;

        /*TODO: delay*/
        for(int32_t delay2, delay1 = 0; delay1 < 3; delay1++)
        {
            delay2++;
        }

        GPIOA->BRR = GPIO_BRR_BR_5;//reset CLK (PA5)
    }
    GPIOA->BSRR = GPIO_BSRR_BS_4;//set SCS (PA4)
}

void spi::Controller::DRV8323_setup()
{
    GPIOC->BSRR = GPIO_BSRR_BS_4;/*set DRV8323_EN (PC4)*/

    /*TODO: delay*/

    DRV8323_write(ADR_DRV_CTRL, DRV8323regDrvCtrl);

    /*TODO: delay*/

    DRV8323_write(ADR_GATE_DRV_HS, DRV8323regGateDrvHS);

    /*TODO: delay*/

    DRV8323_write(ADR_GATE_DRV_LS, DRV8323regGateDrvLS);

    /*TODO: delay*/

    DRV8323_write(ADR_OCP_CTRL, DRV8323regOcpCtrl);

    /*TODO: delay*/

    DRV8323_write(ADR_CSA_CTRL, DRV8323regCsaCtrl);

    /*TODO: delay*/

}
