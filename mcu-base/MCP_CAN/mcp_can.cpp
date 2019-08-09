/*
  MCP_CAN::Controller.cpp
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.
  2017 Copyright (c) Cory J. Fowler  All Rights Reserved.

  Author: Loovee
  Contributor: Cory J. Fowler
  2017-09-25
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
  1301  USA
*/

#include "../uart/controller.h"
#include "mcp_can.h"
//#include "stm32f0xx_hal_dma.h"
//#include "stm32f0xx_hal_spi.h"
//#include "stm32f0xx_hal_spi_ex.h"
#include "stm32f0xx_hal.h"
#define timeout 10

MCP_CAN::Controller MCP_CAN::CONTROLLER;
static int8_t initst=0;
uint32_t initst1=0;

void MCP_CAN::Controller::spiInit()
{
    /*********************************
     * SPI
     * PA5 -> SPI_CLK
     * PA6 -> SPI_MISO
     * PA7 -> SPI_MOSI
     * PA2 -> SCS Serial chip select. A logic low on this pin enables serial interface communication.
     **********************************/
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOAEN;

    GPIOA   ->MODER     |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);/*10: Alternate function mode*/
    GPIOA   ->OTYPER    &= ~(GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);/*push-pull*/
    GPIOA   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);/* High speed*/
    //GPIOA   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);/*No pull-up, pull-down*/
//    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;/*pull-up*/retVal
    GPIOA   ->AFR[0]    |= 0x00000000;/*AF0 (AFSEL5 = AFSEL6 = AFSEL7 = 0b0000)*/

    /*MCP*/
    GPIOA   ->MODER     |= GPIO_MODER_MODER4_0;/*01: General purpose output mode*/
    GPIOA   ->OTYPER    &= ~GPIO_OTYPER_OT_4;/* push-pull*/
    GPIOA   ->OSPEEDR   |= GPIO_OSPEEDER_OSPEEDR2;/* High speed*/
    //GPIOA   ->PUPDR     &= ~GPIO_PUPDR_PUPDR2;/*No pull-up, pull-down*/
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR4_0;/*pull-up*/

    RCC     ->APB2ENR   |= RCC_APB2ENR_SPI1EN; /*enable clock spi1*/

    /* Master selection,  BR: Fpclk/32 48.000.000/32 = 1500k,CPOL = 0, CPHA = 1, enable SSM*/
    SPI1    ->CR1       |=  SPI_CR1_SSM  |SPI_CR1_SSI | SPI_CR1_MSTR|
                            SPI_CR1_BR_2 |SPI_CR1_BR_1| SPI_CR1_BR_0;


    /*  Slave select output enabled, SPI Motorola mode, 8-bit Rx fifo  */
    SPI1    ->CR2       |= SPI_CR2_DS_0|\
                           SPI_CR2_DS_1|
                           SPI_CR2_DS_2|
                           SPI_CR2_FRXTH;

    SPI1    ->CR1       |= SPI_CR1_SPE; /*  Enable SPI1  */
}

void waitforspi_free()
{
    __IO uint8_t tmpreg;
    uint32_t tickstart=0;
    tickstart = HAL_GetTick();
    /* Control if the TX fifo is empty */
    while((SPI1->SR & SPI_SR_FTLVL)!=0)
    {
        /* Timeout management */
        if ((HAL_GetTick() - tickstart) >=  timeout)
        {
            return;
        }
    }
//    SPI_WaitFifoStateUntilTimeout(hspi, SPI_FLAG_FTLVL, SPI_FTLVL_EMPTY, Timeout, tickstart);
    /* Control the BSY flag */
    tickstart = HAL_GetTick();
     while((SPI1->SR & SPI_SR_BSY)!=0)
     {
         /* Timeout management */
         if ((HAL_GetTick() - tickstart) >=  timeout)
         {
             return;
         }
     }
    /* Control if the RX fifo is empty */
     tickstart = HAL_GetTick();
     while ((SPI1->SR & SPI_SR_FRLVL) != 0)
     {
         tmpreg = *((__IO uint8_t *)&SPI1->DR);
         /* To avoid GCC warning */
         UNUSED(tmpreg);
         /* Timeout management */
         if ((HAL_GetTick() - tickstart) >=  timeout)
         {
             return;
         }
     }
}

void SPI_Transmit(uint8_t *pData)
{
    uint32_t tickstart=0;
    /* Check if the SPI is already enabled */
    if ((SPI1->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
    {
        /* Enable SPI peripheral */
        SPI1->CR1   |= SPI_CR1_SPE; /*  Enable SPI1  */
    }

    *((__IO uint8_t *)&SPI1->DR) = (*pData);
    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();
    while((SPI1->SR & SPI_SR_TXE)!=SPI_SR_TXE)
    {
        /* Timeout management */
        if ((HAL_GetTick() - tickstart) >=  timeout)
        {
            return;
        }
    }

    waitforspi_free();
}

void SPI_Receive(uint8_t *pData)
{
    uint32_t tickstart=0;

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    if((SPI1->SR&SPI_SR_TXE)==SPI_SR_TXE)
    {
        *(__IO uint8_t *)&SPI1->DR = 0x0;//(*pData);
    }

    while(!(SPI1->SR & SPI_SR_RXNE)){}
    {
        /* Timeout management */
        if ((HAL_GetTick() - tickstart) >=  timeout)
        {
            return;
        }
    }

    (*(uint8_t *)pData) = *(__IO uint8_t *)&SPI1->DR;

    waitforspi_free();
}


void MCP_CAN::Controller::spi_readwrite(uint8_t cmd)
{
    //SPI_HandleTypeDef hspi1;
    SPI_Transmit(&cmd);
//    HAL_SPI_Transmit(&hspi1,&cmd,1,10);
}

uint8_t MCP_CAN::Controller::spi_read()
{
   // SPI_HandleTypeDef hspi1;
    uint8_t bSpiData;
    SPI_Receive(&bSpiData);
//    HAL_SPI_TransmitReceive(&hspi1,&bSpiData,&bSpiData,1,10);
    return bSpiData;
}



uint8_t MCP_CAN::Controller::MCP2515_ReadByte(uint8_t address)
{
    uint8_t retVal;

    MCP2515_SELECT();

    MCP_CAN::CONTROLLER.spi_readwrite(MCP_READ);
    MCP_CAN::CONTROLLER.spi_readwrite(address);
    retVal = MCP_CAN::CONTROLLER.spi_read();

    MCP2515_UNSELECT();

    return retVal;
}

void MCP_CAN::Controller::Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}
/*********************************************************************************************************
** Function name:           mcp2515_reset
** Descriptions:            Performs a software reset
*********************************************************************************************************/


void MCP_CAN::Controller::Controller::mcp2515_reset(void)
{
//    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_RESET);
    MCP2515_UNSELECT();

//    SPI.endTransaction();
    HAL_Delay(1);
}

/*********************************************************************************************************
** Function name:           mcp2515_readRegister
** Descriptions:            Read data register
*********************************************************************************************************/
int8_t MCP_CAN::Controller::mcp2515_readRegister(const int8_t address)                                                                     
{
    int8_t ret;
    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_READ);
    MCP_CAN::CONTROLLER.spi_readwrite(address);
    ret = MCP_CAN::CONTROLLER.spi_read();
    MCP2515_UNSELECT();

//    SPI.endTransaction();

    return ret;
}

/*********************************************************************************************************
** Function name:           mcp2515_readRegisterS
** Descriptions:            Reads sucessive data registers
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_readRegisterS(const int8_t address, int8_t values[], const int8_t n)
{
    int8_t i;

    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_READ);
    MCP_CAN::CONTROLLER.spi_readwrite(address);
    // mcp2515 has auto-increment of address-pointer
    for (i=0; i<n; i++) 
        values[i] = MCP_CAN::CONTROLLER.spi_read();

    MCP2515_UNSELECT();
//    SPI.endTransaction();
}

/*********************************************************************************************************
** Function name:           mcp2515_setRegister
** Descriptions:            Sets data register
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_setRegister(const int8_t address, const int8_t value)
{

    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_WRITE);
    MCP_CAN::CONTROLLER.spi_readwrite(address);
    MCP_CAN::CONTROLLER.spi_readwrite(value);
    MCP2515_UNSELECT();
//    SPI.endTransaction();
}

/*********************************************************************************************************
** Function name:           mcp2515_setRegisterS
** Descriptions:            Sets sucessive data registers
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_setRegisterS(const int8_t address, const int8_t values[], const int8_t n)
{
    int8_t i;

    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_WRITE);
    MCP_CAN::CONTROLLER.spi_readwrite(address);
       
    for (i=0; i<n; i++) 
        MCP_CAN::CONTROLLER.spi_readwrite(values[i]);
	
    MCP2515_UNSELECT();
//    SPI.endTransaction();
}

/*********************************************************************************************************
** Function name:           mcp2515_modifyRegister
** Descriptions:            Sets specific bits of a register
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_modifyRegister(const int8_t address, const int8_t mask, const int8_t data)
{

    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_BITMOD);
    MCP_CAN::CONTROLLER.spi_readwrite(address);
    MCP_CAN::CONTROLLER.spi_readwrite(mask);
    MCP_CAN::CONTROLLER.spi_readwrite(data);
    MCP2515_UNSELECT();
//    SPI.endTransaction();
}

/*********************************************************************************************************
** Function name:           mcp2515_readStatus
** Descriptions:            Reads status register
*********************************************************************************************************/
int8_t MCP_CAN::Controller::mcp2515_readStatus(void)
{
    int8_t i;

    MCP2515_SELECT();
    MCP_CAN::CONTROLLER.spi_readwrite(MCP_READ_STATUS);
    i = MCP_CAN::CONTROLLER.spi_read();

    MCP2515_UNSELECT();
//    SPI.endTransaction();
    return i;
}

/*********************************************************************************************************
** Function name:           setMode
** Descriptions:            Sets control mode
*********************************************************************************************************/
int8_t MCP_CAN::Controller::setMode(const int8_t opMode)
{
    mcpMode = opMode;
    return mcp2515_setCANCTRL_Mode(mcpMode);
}

/*********************************************************************************************************
** Function name:           mcp2515_setCANCTRL_Mode
** Descriptions:            Set control mode
*********************************************************************************************************/
int8_t MCP_CAN::Controller::mcp2515_setCANCTRL_Mode(int8_t newmode)
{
    int8_t i;
    mcp2515_modifyRegister(MCP_CANCTRL, MODE_MASK, newmode);

    i = mcp2515_readRegister(MCP_CANCTRL);

    i &= MODE_MASK;

    if ( i == newmode ) 
    {

        return MCP2515_OK;
    }
    return MCP2515_FAIL;
}

/*********************************************************************************************************
** Function name:           mcp2515_configRate
** Descriptions:            Set baudrate
*********************************************************************************************************/
int8_t MCP_CAN::Controller::mcp2515_configRate(const int8_t canSpeed, const int8_t canClock)            
{
    int8_t set, cfg1, cfg2, cfg3;
    set = 1;
    switch (canClock)
    {
        case (MCP_8MHZ):
        switch (canSpeed) 
        {
            case (CAN_5KBPS):                                               //   5KBPS                  
            cfg1 = MCP_8MHz_5kBPS_CFG1;
            cfg2 = MCP_8MHz_5kBPS_CFG2;
            cfg3 = MCP_8MHz_5kBPS_CFG3;
            break;

            case (CAN_10KBPS):                                              //  10KBPS                  
            cfg1 = MCP_8MHz_10kBPS_CFG1;
            cfg2 = MCP_8MHz_10kBPS_CFG2;
            cfg3 = MCP_8MHz_10kBPS_CFG3;
            break;

            case (CAN_20KBPS):                                              //  20KBPS                  
            cfg1 = MCP_8MHz_20kBPS_CFG1;
            cfg2 = MCP_8MHz_20kBPS_CFG2;
            cfg3 = MCP_8MHz_20kBPS_CFG3;
            break;

            case (CAN_31K25BPS):                                            //  31.25KBPS                  
            cfg1 = MCP_8MHz_31k25BPS_CFG1;
            cfg2 = MCP_8MHz_31k25BPS_CFG2;
            cfg3 = MCP_8MHz_31k25BPS_CFG3;
            break;

            case (CAN_33K3BPS):                                             //  33.33KBPS                  
            cfg1 = MCP_8MHz_33k3BPS_CFG1;
            cfg2 = MCP_8MHz_33k3BPS_CFG2;
            cfg3 = MCP_8MHz_33k3BPS_CFG3;
            break;

            case (CAN_40KBPS):                                              //  40Kbps
            cfg1 = MCP_8MHz_40kBPS_CFG1;
            cfg2 = MCP_8MHz_40kBPS_CFG2;
            cfg3 = MCP_8MHz_40kBPS_CFG3;
            break;

            case (CAN_50KBPS):                                              //  50Kbps
            cfg1 = MCP_8MHz_50kBPS_CFG1;
            cfg2 = MCP_8MHz_50kBPS_CFG2;
            cfg3 = MCP_8MHz_50kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cfg1 = MCP_8MHz_80kBPS_CFG1;
            cfg2 = MCP_8MHz_80kBPS_CFG2;
            cfg3 = MCP_8MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cfg1 = MCP_8MHz_100kBPS_CFG1;
            cfg2 = MCP_8MHz_100kBPS_CFG2;
            cfg3 = MCP_8MHz_100kBPS_CFG3;
            break;

            case (CAN_125KBPS):                                             // 125Kbps
            cfg1 = MCP_8MHz_125kBPS_CFG1;
            cfg2 = MCP_8MHz_125kBPS_CFG2;
            cfg3 = MCP_8MHz_125kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cfg1 = MCP_8MHz_200kBPS_CFG1;
            cfg2 = MCP_8MHz_200kBPS_CFG2;
            cfg3 = MCP_8MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cfg1 = MCP_8MHz_250kBPS_CFG1;
            cfg2 = MCP_8MHz_250kBPS_CFG2;
            cfg3 = MCP_8MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cfg1 = MCP_8MHz_500kBPS_CFG1;
            cfg2 = MCP_8MHz_500kBPS_CFG2;
            cfg3 = MCP_8MHz_500kBPS_CFG3;
            break;
        
            case (CAN_1000KBPS):                                            //   1Mbps
            cfg1 = MCP_8MHz_1000kBPS_CFG1;
            cfg2 = MCP_8MHz_1000kBPS_CFG2;
            cfg3 = MCP_8MHz_1000kBPS_CFG3;
            break;  

            default:
            set = 0;
	    return MCP2515_FAIL;
            break;
        }
        break;

        case (MCP_16MHZ):
        switch (canSpeed) 
        {
            case (CAN_5KBPS):                                               //   5Kbps
            cfg1 = MCP_16MHz_5kBPS_CFG1;
            cfg2 = MCP_16MHz_5kBPS_CFG2;
            cfg3 = MCP_16MHz_5kBPS_CFG3;
            break;

            case (CAN_10KBPS):                                              //  10Kbps
            cfg1 = MCP_16MHz_10kBPS_CFG1;
            cfg2 = MCP_16MHz_10kBPS_CFG2;
            cfg3 = MCP_16MHz_10kBPS_CFG3;
            break;

            case (CAN_20KBPS):                                              //  20Kbps
            cfg1 = MCP_16MHz_20kBPS_CFG1;
            cfg2 = MCP_16MHz_20kBPS_CFG2;
            cfg3 = MCP_16MHz_20kBPS_CFG3;
            break;

            case (CAN_33K3BPS):                                              //  20Kbps
            cfg1 = MCP_16MHz_33k3BPS_CFG1;
            cfg2 = MCP_16MHz_33k3BPS_CFG2;
            cfg3 = MCP_16MHz_33k3BPS_CFG3;
            break;

            case (CAN_40KBPS):                                              //  40Kbps
            cfg1 = MCP_16MHz_40kBPS_CFG1;
            cfg2 = MCP_16MHz_40kBPS_CFG2;
            cfg3 = MCP_16MHz_40kBPS_CFG3;
            break;

            case (CAN_50KBPS):                                              //  50Kbps
            cfg2 = MCP_16MHz_50kBPS_CFG2;
            cfg3 = MCP_16MHz_50kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cfg1 = MCP_16MHz_80kBPS_CFG1;
            cfg2 = MCP_16MHz_80kBPS_CFG2;
            cfg3 = MCP_16MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cfg1 = MCP_16MHz_100kBPS_CFG1;
            cfg2 = MCP_16MHz_100kBPS_CFG2;
            cfg3 = MCP_16MHz_100kBPS_CFG3;
            break;

            case (CAN_125KBPS):                                             // 125Kbps
            cfg1 = MCP_16MHz_125kBPS_CFG1;
            cfg2 = MCP_16MHz_125kBPS_CFG2;
            cfg3 = MCP_16MHz_125kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cfg1 = MCP_16MHz_200kBPS_CFG1;
            cfg2 = MCP_16MHz_200kBPS_CFG2;
            cfg3 = MCP_16MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cfg1 = MCP_16MHz_250kBPS_CFG1;
            cfg2 = MCP_16MHz_250kBPS_CFG2;
            cfg3 = MCP_16MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cfg1 = MCP_16MHz_500kBPS_CFG1;
            cfg2 = MCP_16MHz_500kBPS_CFG2;
            cfg3 = MCP_16MHz_500kBPS_CFG3;
            break;
        
            case (CAN_1000KBPS):                                            //   1Mbps
            cfg1 = MCP_16MHz_1000kBPS_CFG1;
            cfg2 = MCP_16MHz_1000kBPS_CFG2;
            cfg3 = MCP_16MHz_1000kBPS_CFG3;
            break;  

            default:
            set = 0;
	    return MCP2515_FAIL;
            break;
        }
        break;
        
        case (MCP_20MHZ):
        switch (canSpeed) 
        {
            case (CAN_40KBPS):                                              //  40Kbps
            cfg1 = MCP_20MHz_40kBPS_CFG1;
            cfg2 = MCP_20MHz_40kBPS_CFG2;
            cfg3 = MCP_20MHz_40kBPS_CFG3;
            break;

            case (CAN_50KBPS):                                              //  50Kbps
            cfg1 = MCP_20MHz_50kBPS_CFG1;
            cfg2 = MCP_20MHz_50kBPS_CFG2;
            cfg3 = MCP_20MHz_50kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cfg1 = MCP_20MHz_80kBPS_CFG1;
            cfg2 = MCP_20MHz_80kBPS_CFG2;
            cfg3 = MCP_20MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cfg1 = MCP_20MHz_100kBPS_CFG1;
            cfg2 = MCP_20MHz_100kBPS_CFG2;
            cfg3 = MCP_20MHz_100kBPS_CFG3;
            break;

            case (CAN_125KBPS):                                             // 125Kbps
            cfg1 = MCP_20MHz_125kBPS_CFG1;
            cfg2 = MCP_20MHz_125kBPS_CFG2;
            cfg3 = MCP_20MHz_125kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cfg1 = MCP_20MHz_200kBPS_CFG1;
            cfg2 = MCP_20MHz_200kBPS_CFG2;
            cfg3 = MCP_20MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cfg1 = MCP_20MHz_250kBPS_CFG1;
            cfg2 = MCP_20MHz_250kBPS_CFG2;
            cfg3 = MCP_20MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cfg1 = MCP_20MHz_500kBPS_CFG1;
            cfg2 = MCP_20MHz_500kBPS_CFG2;
            cfg3 = MCP_20MHz_500kBPS_CFG3;
            break;
        
            case (CAN_1000KBPS):                                            //   1Mbps
            cfg1 = MCP_20MHz_1000kBPS_CFG1;
            cfg2 = MCP_20MHz_1000kBPS_CFG2;
            cfg3 = MCP_20MHz_1000kBPS_CFG3;
            break;  

            default:
            set = 0;
            return MCP2515_FAIL;
            break;
        }
        break;
        
        default:
        set = 0;
	return MCP2515_FAIL;
        break;
    }

    if (set) {
        mcp2515_setRegister(MCP_CNF1, cfg1);
        mcp2515_setRegister(MCP_CNF2, cfg2);
        mcp2515_setRegister(MCP_CNF3, cfg3);
        return MCP2515_OK;
    }
     
    return MCP2515_FAIL;
}

/*********************************************************************************************************
** Function name:           mcp2515_initCANBuffers
** Descriptions:            Initialize Buffers, Masks, and Filters
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_initCANBuffers(void)
{
    int8_t i, a1, a2, a3;
    
    int8_t std = 0;               
    int8_t ext = 1;
    INT32U ulMask = 0x00, ulFilt = 0x00;


    mcp2515_write_mf(MCP_RXM0SIDH, ext, ulMask);			/*Set both masks to 0           */
    mcp2515_write_mf(MCP_RXM1SIDH, ext, ulMask);			/*Mask register ignores ext bit */
    
                                                                        /* Set all filters to 0         */
    mcp2515_write_mf(MCP_RXF0SIDH, ext, ulFilt);			/* RXB0: extended               */
    mcp2515_write_mf(MCP_RXF1SIDH, std, ulFilt);			/* RXB1: standard               */
    mcp2515_write_mf(MCP_RXF2SIDH, ext, ulFilt);			/* RXB2: extended               */
    mcp2515_write_mf(MCP_RXF3SIDH, std, ulFilt);			/* RXB3: standard               */
    mcp2515_write_mf(MCP_RXF4SIDH, ext, ulFilt);
    mcp2515_write_mf(MCP_RXF5SIDH, std, ulFilt);

                                                                        /* Clear, deactivate the three  */
                                                                        /* transmit buffers             */
                                                                        /* TXBnCTRL -> TXBnD7           */
    a1 = MCP_TXB0CTRL;
    a2 = MCP_TXB1CTRL;
    a3 = MCP_TXB2CTRL;
    for (i = 0; i < 14; i++) {                                          /* in-buffer loop               */
        mcp2515_setRegister(a1, 0);
        mcp2515_setRegister(a2, 0);
        mcp2515_setRegister(a3, 0);
        a1++;
        a2++;
        a3++;
    }
    mcp2515_setRegister(MCP_RXB0CTRL, 0);
    mcp2515_setRegister(MCP_RXB1CTRL, 0);
}

/*********************************************************************************************************
** Function name:           mcp2515_init
** Descriptions:            Initialize the controller
*********************************************************************************************************/
int8_t MCP_CAN::Controller::mcp2515_init(const int8_t canIDMode, const int8_t canSpeed, const int8_t canClock)
{

  int8_t res;

    mcp2515_reset();

    mcpMode = MCP_LOOPBACK;

    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);

    if(res > 0)
    {
      return res;
    }


    // Set Baudrate
    if(mcp2515_configRate(canSpeed, canClock))
    {

      return res;
    }

    if ( res == MCP2515_OK ) {

                                                                        /* init canbuffers              */
        mcp2515_initCANBuffers();

                                                                        /* interrupt mode               */
        mcp2515_setRegister(MCP_CANINTE, MCP_RX0IF | MCP_RX1IF);

	//Sets BF pins as GPO
	mcp2515_setRegister(MCP_BFPCTRL,MCP_BxBFS_MASK | MCP_BxBFE_MASK);
	//Sets RTS pins as GPI
	mcp2515_setRegister(MCP_TXRTSCTRL,0x00);

        switch(canIDMode)
        {
            case (MCP_ANY):
            mcp2515_modifyRegister(MCP_RXB0CTRL,
            MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
            MCP_RXB_RX_ANY | MCP_RXB_BUKT_MASK);
            mcp2515_modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
            MCP_RXB_RX_ANY);
            break;
/*          The followingn two functions of the MCP2515 do not work, there is a bug in the silicon.
            case (MCP_STD): 
            mcp2515_modifyRegister(MCP_RXB0CTRL,
            MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
            MCP_RXB_RX_STD | MCP_RXB_BUKT_MASK );
            mcp2515_modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
            MCP_RXB_RX_STD);
            break;

            case (MCP_EXT): 
            mcp2515_modifyRegister(MCP_RXB0CTRL,
            MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
            MCP_RXB_RX_EXT | MCP_RXB_BUKT_MASK );
            mcp2515_modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
            MCP_RXB_RX_EXT);
            break;
*/
            case (MCP_STDEXT): 
            mcp2515_modifyRegister(MCP_RXB0CTRL,
            MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
            MCP_RXB_RX_STDEXT | MCP_RXB_BUKT_MASK );
            mcp2515_modifyRegister(MCP_RXB1CTRL, MCP_RXB_RX_MASK,
            MCP_RXB_RX_STDEXT);
            break;
    
            default:

            return MCP2515_FAIL;
            break;
}    


        res = mcp2515_setCANCTRL_Mode(mcpMode);                                                                
        if(res)
        {

          return res;
        }

    }
    return res;

}

/*********************************************************************************************************
** Function name:           mcp2515_write_id
** Descriptions:            Write CAN ID
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_write_id( const int8_t mcp_addr, const int8_t ext, const INT32U id )
{
    uint16_t canid;
    int8_t tbufdata[4];

    canid = (uint16_t)(id & 0x0FFFF);

    if ( ext == 1) 
    {
        tbufdata[MCP_EID0] = (int8_t) (canid & 0xFF);
        tbufdata[MCP_EID8] = (int8_t) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        tbufdata[MCP_SIDL] = (int8_t) (canid & 0x03);
        tbufdata[MCP_SIDL] += (int8_t) ((canid & 0x1C) << 3);
        tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
        tbufdata[MCP_SIDH] = (int8_t) (canid >> 5 );
    }
    else 
    {
        tbufdata[MCP_SIDH] = (int8_t) (canid >> 3 );
        tbufdata[MCP_SIDL] = (int8_t) ((canid & 0x07 ) << 5);
        tbufdata[MCP_EID0] = 0;
        tbufdata[MCP_EID8] = 0;
    }
    
    mcp2515_setRegisterS( mcp_addr, tbufdata, 4 );
}

/*********************************************************************************************************
** Function name:           mcp2515_write_mf
** Descriptions:            Write Masks and Filters
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_write_mf( const int8_t mcp_addr, const int8_t ext, const INT32U id )
{
    uint16_t canid;
    int8_t tbufdata[4];

    canid = (uint16_t)(id & 0x0FFFF);

    if ( ext == 1) 
    {
        tbufdata[MCP_EID0] = (int8_t) (canid & 0xFF);
        tbufdata[MCP_EID8] = (int8_t) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        tbufdata[MCP_SIDL] = (int8_t) (canid & 0x03);
        tbufdata[MCP_SIDL] += (int8_t) ((canid & 0x1C) << 3);
        tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
        tbufdata[MCP_SIDH] = (int8_t) (canid >> 5 );
    }
    else 
    {
        tbufdata[MCP_EID0] = (int8_t) (canid & 0xFF);
        tbufdata[MCP_EID8] = (int8_t) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        tbufdata[MCP_SIDL] = (int8_t) ((canid & 0x07) << 5);
        tbufdata[MCP_SIDH] = (int8_t) (canid >> 3 );
    }
    
    mcp2515_setRegisterS( mcp_addr, tbufdata, 4 );
}

/*********************************************************************************************************
** Function name:           mcp2515_read_id
** Descriptions:            Read CAN ID
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_read_id( const int8_t mcp_addr, int8_t* ext, INT32U* id )
{
    int8_t tbufdata[4];

    *ext = 0;
    *id = 0;

    mcp2515_readRegisterS( mcp_addr, tbufdata, 4 );

    *id = (tbufdata[MCP_SIDH]<<3) + (tbufdata[MCP_SIDL]>>5);

    if ( (tbufdata[MCP_SIDL] & MCP_TXB_EXIDE_M) ==  MCP_TXB_EXIDE_M ) 
    {
                                                                        /* extended id                  */
        *id = (*id<<2) + (tbufdata[MCP_SIDL] & 0x03);
        *id = (*id<<8) + tbufdata[MCP_EID8];
        *id = (*id<<8) + tbufdata[MCP_EID0];
        *ext = 1;
    }
}

/*********************************************************************************************************
** Function name:           mcp2515_write_canMsg
** Descriptions:            Write message
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_write_canMsg( const int8_t buffer_sidh_addr)
{
    int8_t mcp_addr;
    mcp_addr = buffer_sidh_addr;
    mcp2515_setRegisterS(mcp_addr+5, m_nDta, m_nDlc );                  /* write data bytes             */
	
    if ( m_nRtr == 1)                                                   /* if RTR set bit in byte       */
        m_nDlc |= MCP_RTR_MASK;  

    mcp2515_setRegister((mcp_addr+4), m_nDlc );                         /* write the RTR and DLC        */
    mcp2515_write_id(mcp_addr, m_nExtFlg, m_nID );                      /* write CAN id                 */

}

/*********************************************************************************************************
** Function name:           mcp2515_read_canMsg
** Descriptions:            Read message
*********************************************************************************************************/
void MCP_CAN::Controller::mcp2515_read_canMsg( const int8_t buffer_sidh_addr)        /* read can msg                 */
{
    int8_t mcp_addr, ctrl;

    mcp_addr = buffer_sidh_addr;

    mcp2515_read_id( mcp_addr, &m_nExtFlg,&m_nID );

    ctrl = mcp2515_readRegister( mcp_addr-1 );
    m_nDlc = mcp2515_readRegister( mcp_addr+4 );

    if (ctrl & 0x08)
        m_nRtr = 1;
    else
        m_nRtr = 0;

    m_nDlc &= MCP_DLC_MASK;
    mcp2515_readRegisterS( mcp_addr+5, &(m_nDta[0]), m_nDlc );
}

/*********************************************************************************************************
** Function name:           mcp2515_getNextFreeTXBuf
** Descriptions:            Send message
*********************************************************************************************************/
int8_t MCP_CAN::Controller::mcp2515_getNextFreeTXBuf(int8_t *txbuf_n)                 /* get Next free txbuf          */
{
    int8_t res, i, ctrlval;
    int8_t ctrlregs[MCP_N_TXBUFFERS] = { MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL };

    res = MCP_ALLTXBUSY;
    *txbuf_n = 0x00;

                                                                        /* check all 3 TX-Buffers       */
    for (i=0; i<MCP_N_TXBUFFERS; i++) {
        ctrlval = mcp2515_readRegister( ctrlregs[i] );
        if ( (ctrlval & MCP_TXB_TXREQ_M) == 0 ) {
            *txbuf_n = ctrlregs[i]+1;                                   /* return SIDH-address of Buffer*/
            
            res = MCP2515_OK;
            return res;                                                 /* ! function exit              */
        }
    }
    return res;
}

/*********************************************************************************************************
** Function name:           begin
** Descriptions:            Public function to declare controller initialization parameters.
*********************************************************************************************************/
int8_t MCP_CAN::Controller::begin(int8_t idmodeset, int8_t speedset, int8_t clockset)
{
    int8_t res;

    res = mcp2515_init(idmodeset, speedset, clockset);
    if (res == MCP2515_OK)
        return CAN_OK;

    return CAN_FAILINIT;
}

/*********************************************************************************************************
** Function name:           init_Mask
** Descriptions:            Public function to set mask(s).
*********************************************************************************************************/
int8_t MCP_CAN::Controller::init_Mask(int8_t num, int8_t ext, INT32U ulData)
{
    int8_t res = MCP2515_OK;

    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0){

	return res;
     }
    
    if (num == 0){
        mcp2515_write_mf(MCP_RXM0SIDH, ext, ulData);

    }
    else if(num == 1){
        mcp2515_write_mf(MCP_RXM1SIDH, ext, ulData);
    }
    else res =  MCP2515_FAIL;
    
    res = mcp2515_setCANCTRL_Mode(mcpMode);
    if(res > 0){

	return res;
    }

    return res;
}

/*********************************************************************************************************
** Function name:           init_Mask
** Descriptions:            Public function to set mask(s).
*********************************************************************************************************/
int8_t MCP_CAN::Controller::init_Mask(int8_t num, INT32U ulData)
{
    int8_t res = MCP2515_OK;
    int8_t ext = 0;

    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0){

  return res;
}
    
    if((ulData & 0x80000000) == 0x80000000)
        ext = 1;
    
    if (num == 0){
        mcp2515_write_mf(MCP_RXM0SIDH, ext, ulData);

    }
    else if(num == 1){
        mcp2515_write_mf(MCP_RXM1SIDH, ext, ulData);
    }
    else res =  MCP2515_FAIL;
    
    res = mcp2515_setCANCTRL_Mode(mcpMode);
    if(res > 0){

    return res;
  }

    return res;
}

/*********************************************************************************************************
** Function name:           init_Filt
** Descriptions:            Public function to set filter(s).
*********************************************************************************************************/
int8_t MCP_CAN::Controller::init_Filt(int8_t num, int8_t ext, INT32U ulData)
{
    int8_t res = MCP2515_OK;

    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0)
    {

      return res;
    }
    
    switch( num )
    {
        case 0:
        mcp2515_write_mf(MCP_RXF0SIDH, ext, ulData);
        break;

        case 1:
        mcp2515_write_mf(MCP_RXF1SIDH, ext, ulData);
        break;

        case 2:
        mcp2515_write_mf(MCP_RXF2SIDH, ext, ulData);
        break;

        case 3:
        mcp2515_write_mf(MCP_RXF3SIDH, ext, ulData);
        break;

        case 4:
        mcp2515_write_mf(MCP_RXF4SIDH, ext, ulData);
        break;

        case 5:
        mcp2515_write_mf(MCP_RXF5SIDH, ext, ulData);
        break;

        default:
        res = MCP2515_FAIL;
    }
    
    res = mcp2515_setCANCTRL_Mode(mcpMode);
    if(res > 0)
    {
      return res;
    }

    
    return res;
}

/*********************************************************************************************************
** Function name:           init_Filt
** Descriptions:            Public function to set filter(s).
*********************************************************************************************************/
int8_t MCP_CAN::Controller::init_Filt(int8_t num, INT32U ulData)
{
    int8_t res = MCP2515_OK;
    int8_t ext = 0;
    
    res = mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    if(res > 0)
    {

      return res;
    }
    
    if((ulData & 0x80000000) == 0x80000000)
        ext = 1;
    
    switch( num )
    {
        case 0:
        mcp2515_write_mf(MCP_RXF0SIDH, ext, ulData);
        break;

        case 1:
        mcp2515_write_mf(MCP_RXF1SIDH, ext, ulData);
        break;

        case 2:
        mcp2515_write_mf(MCP_RXF2SIDH, ext, ulData);
        break;

        case 3:
        mcp2515_write_mf(MCP_RXF3SIDH, ext, ulData);
        break;

        case 4:
        mcp2515_write_mf(MCP_RXF4SIDH, ext, ulData);
        break;

        case 5:
        mcp2515_write_mf(MCP_RXF5SIDH, ext, ulData);
        break;

        default:
        res = MCP2515_FAIL;
    }
    
    res = mcp2515_setCANCTRL_Mode(mcpMode);
    if(res > 0)
    {

      return res;
    }  
    return res;
}

/*********************************************************************************************************
** Function name:           setMsg
** Descriptions:            Set can message, such as dlc, id, dta[] and so on
*********************************************************************************************************/
int8_t MCP_CAN::Controller::setMsg(INT32U id, int8_t rtr, int8_t ext, int8_t len, int8_t *pData)
{
    int i = 0;
    m_nID     = id;
    m_nRtr    = rtr;
    m_nExtFlg = ext;
    m_nDlc    = len;
    for(i = 0; i<MAX_CHAR_IN_MESSAGE; i++)
        m_nDta[i] = *(pData+i);
	
    return MCP2515_OK;
}

/*********************************************************************************************************
** Function name:           clearMsg
** Descriptions:            Set all messages to zero
*********************************************************************************************************/
int8_t MCP_CAN::Controller::clearMsg()
{
    m_nID       = 0;
    m_nDlc      = 0;
    m_nExtFlg   = 0;
    m_nRtr      = 0;
    m_nfilhit   = 0;
    for(int i = 0; i<m_nDlc; i++ )
      m_nDta[i] = 0x00;

    return MCP2515_OK;
}

/*********************************************************************************************************
** Function name:           sendMsg
** Descriptions:            Send message
*********************************************************************************************************/
int8_t MCP_CAN::Controller::sendMsg()
{
    int8_t res, res1, txbuf_n;
    uint16_t uiTimeOut = 0;

    do {
        res = mcp2515_getNextFreeTXBuf(&txbuf_n);                       /* info = addr.                 */
        uiTimeOut++;
    } while (res == MCP_ALLTXBUSY && (uiTimeOut < TIMEOUTVALUE));

    if(uiTimeOut == TIMEOUTVALUE)
    {
        return CAN_GETTXBFTIMEOUT;                                      /* get tx buff time out         */
    }
    uiTimeOut = 0;
    mcp2515_write_canMsg( txbuf_n);
    mcp2515_modifyRegister( txbuf_n-1 , MCP_TXB_TXREQ_M, MCP_TXB_TXREQ_M );
    
    do
    {
        uiTimeOut++;
        res1 = mcp2515_readRegister(txbuf_n-1);                         /* read send buff ctrl reg 	*/
        res1 = res1 & 0x08;
    } while (res1 && (uiTimeOut < TIMEOUTVALUE));
    
    if(uiTimeOut == TIMEOUTVALUE)                                       /* send msg timeout             */
        return CAN_SENDMSGTIMEOUT;
    
    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message to transmitt buffer
*********************************************************************************************************/
int8_t MCP_CAN::Controller::sendMsgBuf(INT32U id, int8_t ext, int8_t len, int8_t *buf)
{
    int8_t res;
	
    setMsg(id, 0, ext, len, buf);
    res = sendMsg();
    
    return res;
}

/*********************************************************************************************************
** Function name:           sendMsgBuf
** Descriptions:            Send message to transmitt buffer
*********************************************************************************************************/
int8_t MCP_CAN::Controller::sendMsgBuf(INT32U id, int8_t len, int8_t *buf)
{
    int8_t ext = 0, rtr = 0;
    int8_t res;
    
    if((id & 0x80000000) == 0x80000000)
        ext = 1;
 
    if((id & 0x40000000) == 0x40000000)
        rtr = 1;
        
    setMsg(id, rtr, ext, len, buf);
    res = sendMsg();
    
    return res;
}

/*********************************************************************************************************
** Function name:           readMsg
** Descriptions:            Read message
*********************************************************************************************************/
int8_t MCP_CAN::Controller::readMsg()
{
    int8_t stat, res;

    stat = mcp2515_readStatus();

    if ( stat & MCP_STAT_RX0IF )                                        /* Msg in Buffer 0              */
    {
        mcp2515_read_canMsg( MCP_RXBUF_0);
        mcp2515_modifyRegister(MCP_CANINTF, MCP_RX0IF, 0);
        res = CAN_OK;
    }
    else if ( stat & MCP_STAT_RX1IF )                                   /* Msg in Buffer 1              */
    {
        mcp2515_read_canMsg( MCP_RXBUF_1);
        mcp2515_modifyRegister(MCP_CANINTF, MCP_RX1IF, 0);
        res = CAN_OK;
    }
    else 
        res = CAN_NOMSG;
    
    return res;
}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            Public function, Reads message from receive buffer.
*********************************************************************************************************/
int8_t MCP_CAN::Controller::readMsgBuf(INT32U *id, int8_t *ext, int8_t *len, int8_t buf[])
{
    if(readMsg() == CAN_NOMSG)
	return CAN_NOMSG;
	
    *id  = m_nID;
    *len = m_nDlc;
    *ext = m_nExtFlg;
    for(int i = 0; i<m_nDlc; i++)
        buf[i] = m_nDta[i];

    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           readMsgBuf
** Descriptions:            Public function, Reads message from receive buffer.
*********************************************************************************************************/
int8_t MCP_CAN::Controller::readMsgBuf(INT32U *id, int8_t *len, int8_t buf[])
{
    if(readMsg() == CAN_NOMSG)
	return CAN_NOMSG;

    if (m_nExtFlg)
        m_nID |= 0x80000000;

    if (m_nRtr)
        m_nID |= 0x40000000;
	
    *id  = m_nID;
    *len = m_nDlc;
    
    for(int i = 0; i<m_nDlc; i++)
        buf[i] = m_nDta[i];

    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           checkReceive
** Descriptions:            Public function, Checks for received data.  (Used if not using the interrupt output)
*********************************************************************************************************/
int8_t MCP_CAN::Controller::checkReceive(void)
{
    int8_t res;
    res = mcp2515_readStatus();                                         /* RXnIF in Bit 1 and 0         */
    if ( res & MCP_STAT_RXIF_MASK )
        return CAN_MSGAVAIL;
    else 
        return CAN_NOMSG;
}

/*********************************************************************************************************
** Function name:           checkError
** Descriptions:            Public function, Returns error register data.
*********************************************************************************************************/
int8_t MCP_CAN::Controller::checkError(void)
{
    int8_t eflg = mcp2515_readRegister(MCP_EFLG);

    if ( eflg & MCP_EFLG_ERRORMASK ) 
        return CAN_CTRLERROR;
    else
        return CAN_OK;
}

/*********************************************************************************************************
** Function name:           getError
** Descriptions:            Returns error register value.
*********************************************************************************************************/
int8_t MCP_CAN::Controller::getError(void)
{
    return mcp2515_readRegister(MCP_EFLG);
}

/*********************************************************************************************************
** Function name:           mcp2515_errorCountRX
** Descriptions:            Returns REC register value
*********************************************************************************************************/
int8_t MCP_CAN::Controller::errorCountRX(void)                             
{
    return mcp2515_readRegister(MCP_REC);
}

/*********************************************************************************************************
** Function name:           mcp2515_errorCountTX
** Descriptions:            Returns TEC register value
*********************************************************************************************************/
int8_t MCP_CAN::Controller::errorCountTX(void)                             
{
    return mcp2515_readRegister(MCP_TEC);
}

/*********************************************************************************************************
** Function name:           mcp2515_enOneShotTX
** Descriptions:            Enables one shot transmission mode
*********************************************************************************************************/
int8_t MCP_CAN::Controller::enOneShotTX(void)                             
{
    mcp2515_modifyRegister(MCP_CANCTRL, MODE_ONESHOT, MODE_ONESHOT);
    if((mcp2515_readRegister(MCP_CANCTRL) & MODE_ONESHOT) != MODE_ONESHOT)
	    return CAN_FAIL;
    else
	    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           mcp2515_disOneShotTX
** Descriptions:            Disables one shot transmission mode
*********************************************************************************************************/
int8_t MCP_CAN::Controller::disOneShotTX(void)                             
{
    mcp2515_modifyRegister(MCP_CANCTRL, MODE_ONESHOT, 0);
    if((mcp2515_readRegister(MCP_CANCTRL) & MODE_ONESHOT) != 0)
        return CAN_FAIL;
    else
        return CAN_OK;
}

/*********************************************************************************************************
** Function name:           mcp2515_abortTX
** Descriptions:            Aborts any queued transmissions
*********************************************************************************************************/
int8_t MCP_CAN::Controller::abortTX(void)                             
{
    mcp2515_modifyRegister(MCP_CANCTRL, ABORT_TX, ABORT_TX);
	
    // Maybe check to see if the TX buffer transmission request bits are cleared instead?
    if((mcp2515_readRegister(MCP_CANCTRL) & ABORT_TX) != ABORT_TX)
	    return CAN_FAIL;
    else
	    return CAN_OK;
}

/*********************************************************************************************************
** Function name:           setGPO
** Descriptions:            Public function, Checks for r
*********************************************************************************************************/
int8_t MCP_CAN::Controller::setGPO(int8_t data)
{
    mcp2515_modifyRegister(MCP_BFPCTRL, MCP_BxBFS_MASK, (data<<4));
	    
    return 0;
}

/*********************************************************************************************************
** Function name:           getGPI
** Descriptions:            Public function, Checks for r
*********************************************************************************************************/
int8_t MCP_CAN::Controller::getGPI(void)
{
    int8_t res;
    res = mcp2515_readRegister(MCP_TXRTSCTRL) & MCP_BxRTS_MASK;
    return (res >> 3);
}

void MCP_CAN::Controller::init()
{
    uint8_t i,j;

    HAL_InitTick(0);

    MCP_CAN::CONTROLLER.spiInit();

    initst = MCP_CAN::CONTROLLER.begin(MCP_ANY, CAN_33K3BPS, MCP_16MHZ);

    MCP_CAN::CONTROLLER.setMode(MCP_NORMAL);

    MCP_CAN::CONTROLLER.enOneShotTX();
}

static uint32_t count,count0,count1;
int8_t temp;
void MCP_CAN::Controller::loop()
{
    int8_t data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

//    uart::CONTROLLER.printfMessage("MCP_can testing..");
    initst=MCP_CAN::CONTROLLER.sendMsgBuf(0x100, 0, 8, data);
//    temp=MCP_CAN::CONTROLLER.getGPI();
//      temp=MCP2515_ReadByte(0x0D);
//       mcp2515_reset();
//       mcp2515_modifyRegister(MCP_CANCTRL, MODE_MASK, MCP_CANCTRL);
//       initst = mcp2515_readRegister(MCP_CANCTRL)&MODE_MASK;
//    temp=mcp2515_setCANCTRL_Mode(MODE_CONFIG);
    count1=0;
    if(count<5000) count++;
    else
    {
        initst1 = (SPI1->SR & SPI_SR_FRLVL);
        uart::CONTROLLER.printfMessage("MCP_can testing..%x,%x",(uint8_t)initst,initst1);
        count=0;
    }
}

extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/


