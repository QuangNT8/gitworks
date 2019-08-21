#include "stm32f0xx.h"
#include "GPIOexp.h"

#define I2C_NO_STARTSTOP               (0x00000000U)
#define I2C_GENERATE_START_WRITE       I2C_CR2_START
#define I2C_RELOAD_MODE                I2C_CR2_RELOAD
#define I2C_AUTOEND_MODE               I2C_CR2_AUTOEND
#define I2C_SOFTEND_MODE               (0x00000000U)
#define I2C_MEMADD_SIZE_8BIT            (0x00000001U)

#define I2C_MEM_ADD_MSB(__ADDRESS__)              ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)(0xFF00U))) >> 8U)))
#define I2C_MEM_ADD_LSB(__ADDRESS__)              ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)(0x00FFU))))

#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) (((((__HANDLE__)->Instance->ISR) & (__FLAG__)) == (__FLAG__)) ? SET : RESET)


#define MAX_NBYTE_SIZE      255U
#define OwnAddress1 0U

GPIOexp::Controller GPIOexp::CONTROLLER;


void GPIOexp::Controller::init()
{
    /*GPIO configuration*/
    GPIOF->OSPEEDR  |= GPIO_OSPEEDER_OSPEEDR6|GPIO_OSPEEDER_OSPEEDR7;
    GPIOF->OTYPER   |= GPIO_OTYPER_OT_6|GPIO_OTYPER_OT_7;
    GPIOF->MODER    |= GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1;
    GPIOF->PUPDR    |= GPIO_PUPDR_PUPDR6_0|GPIO_PUPDR_PUPDR7_0;
    /*I2C configuration*/
    RCC->AHBENR     |= RCC_AHBENR_GPIOFEN;
    RCC->APB1ENR    |= RCC_APB1ENR_I2C2EN; // enable clock for I2C2
    I2C2->TIMINGR   |= (0x2010091A)&(0xF0FFFFFFU);

    I2C2->OAR1      &= ~I2C_OAR1_OA1EN; /* Disable Own Address1 before set the Own Address1 configuration */
    I2C2->OAR1      |= I2C_OAR1_OA1EN|OwnAddress1; /* Configure I2C2: Own Address1 and ack own address1 mode */
    /* Enable the AUTOEND by default, and enable NACK (should be disable only during Slave process */
    I2C2->CR2       |= (I2C_CR2_AUTOEND | I2C_CR2_NACK);
    /* Disable Own Address2 */
    I2C2->OAR2      &= ~I2C_OAR2_OA2EN;
    /* Enable the selected I2C peripheral */
    I2C2->CR1       |= I2C_CR1_PE;
}


void I2C_TransferConfig(uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
    uint32_t tmpreg = 0U;
    /* Get the CR2 register value */
    tmpreg = I2C2->CR2;
     /* clear tmpreg specific bits */
    tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD |\
                I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
    /* update tmpreg */
     tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16) & I2C_CR2_NBYTES) | \
                          (uint32_t)Mode | (uint32_t)Request);
    /* update CR2 register */
    I2C2->CR2 = tmpreg;
    I2C2->CR2 = I2C_CR2_START;
}

bool I2C_WaitOnTXISFlagUntilTimeout(uint32_t Timeoutin)
{
    uint32_t timeout=0;
    while(((I2C2->ISR&I2C_ISR_TXIS) == RESET)&&(timeout<Timeoutin))
    {
        timeout++;
    }
    if(timeout>=Timeoutin) return false;
    else return true;
}

bool I2C_WaitOnFlagUntilTimeout(uint32_t Flag, FlagStatus Status, uint32_t Timeoutin)
{
    uint32_t timeout=0;
    while(((I2C2->ISR&Flag) == Status)&&(timeout<Timeoutin))
    {
        timeout++;
    }
    if(timeout>=Timeoutin) return false;
    else return true;
}

bool I2C_RequestMemoryWrite(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout)
{
      I2C_TransferConfig(DevAddress, MemAddSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);

      /* Wait until TXIS flag is set */
      if (I2C_WaitOnTXISFlagUntilTimeout(Timeout) == false)
      {
          return false;
      }

      /* If Memory address size is 8Bit */
    if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
    {
        /* Send Memory Address */
        I2C2->TXDR = I2C_MEM_ADD_LSB(MemAddress);
    }
    /* If Memory address size is 16Bit */
    else
    {
        /* Send MSB of Memory Address */
        I2C2->TXDR = I2C_MEM_ADD_MSB(MemAddress);

        /* Wait until TXIS flag is set */
        if (I2C_WaitOnTXISFlagUntilTimeout(Timeout) == false)
        {
            return false;
        }
        /* Send LSB of Memory Address */
        I2C2->TXDR = I2C_MEM_ADD_LSB(MemAddress);
    }

      /* Wait until TCR flag is set */
    if (I2C_WaitOnFlagUntilTimeout(I2C_ISR_TCR,RESET,Timeout) == false)
    {
        return false;
    }
    return true;
}



void GPIOexp::Controller::I2C_Master_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint16_t XferCount;
    XferCount = Size;
    if (XferCount > MAX_NBYTE_SIZE)
    {

    }
}

bool GPIOexp::Controller::HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    uint16_t XferCount,XferSize;

    if(I2C_WaitOnFlagUntilTimeout(I2C_ISR_BUSY, SET, I2C_TIMEOUT_BUSY) == false)
    {
      return false;
    }

    XferCount = Size;
    if (I2C_RequestMemoryWrite(DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_COMMON) == false)
    {
      return false;
    }
    if(XferCount > MAX_NBYTE_SIZE)
    {
      XferSize = MAX_NBYTE_SIZE;
      I2C_TransferConfig(DevAddress,XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
    }
    else
    {
      XferSize = XferCount;
      I2C_TransferConfig(DevAddress,XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
    }

}



