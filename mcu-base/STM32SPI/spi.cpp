
#include "stm32f0xx.h"

#include "spi.h"

#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_gpio_ex.h"
#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal.h"


/*AS5047*/
#define CLEAR_SCS_AS5047    (GPIOA->BSRR |= GPIO_BSRR_BR_2)
#define SET_SCS_AS5047      (GPIOA->BSRR |= GPIO_BSRR_BS_2)

#define OP_WRITE 0x0000
#define OP_READ  0x4000

#define PARITY_EVEN 0x0000
#define PARITY_ODD  0x8000

#define ADDR_NOP          0x0000
#define ADDR_ERRFL        0x0001
#define ADDR_PROG         0x0003
#define ADDR_DIAAGC       0x3FFC
#define ADDR_MAG          0x3FFD
#define ADDR_ANGLEUNC     0x3FFE
#define ADDR_ANGLECOM     0x3FFF
#define ADDR_SETTINGS1    0x0018
#define ADDR_SETTINGS2    0x0019

#define CMD_R_NOP       (PARITY_ODD  | OP_READ | ADDR_NOP)
#define CMD_R_ERRFL     (PARITY_EVEN | OP_READ | ADDR_ERRFL)
#define CMD_R_PROG      (PARITY_ODD  | OP_READ | ADDR_PROG)
#define CMD_R_DIAAGC    (PARITY_ODD  | OP_READ | ADDR_DIAAGC)
#define CMD_R_MAG       (PARITY_EVEN | OP_READ | ADDR_MAG)
#define CMD_R_ANGLEUNC  (PARITY_EVEN | OP_READ | ADDR_ANGLEUNC)
#define CMD_R_ANGLECOM  (PARITY_ODD  | OP_READ | ADDR_ANGLECOM)
#define CMD_R_SETTINGS1 (PARITY_ODD  | OP_READ | ADDR_SETTINGS1)
#define CMD_R_SETTINGS2 (PARITY_EVEN | OP_READ | ADDR_SETTINGS2)

/**@brief Timer control block. */


spi::Controller spi::CONTROLLER;


void spi::Controller::init_it()
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
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;/*pull-up*/
    GPIOA   ->AFR[0]    |= 0x00000000;/*AF0 (AFSEL5 = AFSEL6 = AFSEL7 = 0b0000)*/

    /*AS5047D*/
    GPIOA   ->MODER     |= GPIO_MODER_MODER2_0;/*01: General purpose output mode*/
    GPIOA   ->OTYPER    &= ~GPIO_OTYPER_OT_2;/* push-pull*/
    GPIOA   ->OSPEEDR   |= GPIO_OSPEEDER_OSPEEDR2;/* High speed*/
    //GPIOA   ->PUPDR     &= ~GPIO_PUPDR_PUPDR2;/*No pull-up, pull-down*/
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR2_0;/*pull-up*/

    RCC     ->APB2ENR   |= RCC_APB2ENR_SPI1EN; /*enable clock spi1*/
    SPI1    ->CR1       |= SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_CPHA | SPI_CR1_SSM; /* Master selection,  BR: Fpclk/32 48.000.000/32 = 1500k,CPOL = 0, CPHA = 1, enable SSM*/
    SPI1    ->CR2       |= SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_DS; /*  Slave select output enabled, RXNE IT, SPI Motorola mode, 16-bit Rx fifo  */
    SPI1    ->CR1       |= SPI_CR1_SPE; /*  Enable SPI1  */

    NVIC_EnableIRQ(SPI1_IRQn); // Enable interrupt from SPI1 (NVIC level)


}

void spi::Controller::init()
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
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;/*pull-up*/
    GPIOA   ->AFR[0]    |= 0x00000000;/*AF0 (AFSEL5 = AFSEL6 = AFSEL7 = 0b0000)*/

    /*AS5047D*/
    GPIOA   ->MODER     |= GPIO_MODER_MODER2_0;/*01: General purpose output mode*/
    GPIOA   ->OTYPER    &= ~GPIO_OTYPER_OT_2;/* push-pull*/
    GPIOA   ->OSPEEDR   |= GPIO_OSPEEDER_OSPEEDR2;/* High speed*/
    //GPIOA   ->PUPDR     &= ~GPIO_PUPDR_PUPDR2;/*No pull-up, pull-down*/
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR2_0;/*pull-up*/

    RCC     ->APB2ENR   |= RCC_APB2ENR_SPI1EN; /*enable clock spi1*/
    SPI1    ->CR1       |= SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_CPHA | SPI_CR1_SSM; /* Master selection,  BR: Fpclk/32 48.000.000/32 = 1500k,CPOL = 0, CPHA = 1, enable SSM*/
    SPI1    ->CR2       |= SPI_CR2_SSOE | SPI_CR2_DS; /*  Slave select output enabled, SPI Motorola mode, 16-bit Rx fifo  */
    SPI1    ->CR1       |= SPI_CR1_SPE; /*  Enable SPI1  */

}

uint16_t spi::Controller::spiRead()
{
    static uint16_t temp;
//    while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY);
    while ((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE);
    SET_SCS_AS5047;
    temp = SPI1->DR;

    return temp;

}

void spi::Controller::spiWrite(uint16_t command)
{
    CLEAR_SCS_AS5047;
    while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
    SPI1->DR = command;
}

uint16_t spi::Controller::spi_WriteRead_byte(uint16_t wr)
{
    CLEAR_SCS_AS5047;
    uint16_t data=0;
    while ((SPI1->SR & SPI_SR_TXE) != SPI_SR_TXE);
    *((__IO uint16_t *)&SPI1->DR) = wr;
    while ((SPI1->SR & SPI_SR_BSY) == SPI_SR_BSY);

    while ((SPI1->SR & SPI_SR_RXNE) != SPI_SR_RXNE);

    data = *((__IO uint16_t *)&SPI1->DR);
    SET_SCS_AS5047;
    return data&0x3FFF;
}


static uint32_t count=0,result=0;
uint32_t spi::Controller::loop()
{
//    uint16_t result=0;
    if(count<2000)
    {
        count++;

    }
    else
    {
        result=spi_WriteRead_byte(CMD_R_ANGLECOM);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);
        count=0;
    }
    return result;
}


void spi::Controller::ledtestinit()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*TEST*/
    /*Configure GPIO pins : LD4_Pin LD3_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


