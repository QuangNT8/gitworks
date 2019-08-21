#include "define.h"
#include "controller.h"
#include "uart/controller.h"

void motor::Controller::initSPI_()
{
    /*********************************
     * SPI
     * PA5 -> SPI_CLK
     * PA6 -> SPI_MISO
     * PA7 -> SPI_MOSI
     * PA4 -> SCS Serial chip select. A logic low on this pin enables serial interface communication.
     **********************************/
    RCC     ->AHBENR    |= RCC_AHBENR_GPIOAEN;

    GPIOA   ->MODER     |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);/*10: Alternate function mode*/
    GPIOA   ->OTYPER    &= ~(GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);/*push-pull*/
    GPIOA   ->OSPEEDR   |= (GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);/* High speed*/
    //GPIOA   ->PUPDR     &= ~(GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);/*No pull-up, pull-down*/
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;/*pull-up*/
    GPIOA   ->AFR[0]    |= 0x00000000;/*AF0 (AFSEL5 = AFSEL6 = AFSEL7 = 0b0000)*/

    /*AS5047D*/
    GPIOA   ->MODER     |= GPIO_MODER_MODER4_0;/*01: General purpose output mode*/
    GPIOA   ->OTYPER    &= ~GPIO_OTYPER_OT_4;/* push-pull*/
    GPIOA   ->OSPEEDR   |= GPIO_OSPEEDER_OSPEEDR4;/* High speed*/
    //GPIOA   ->PUPDR     &= ~GPIO_PUPDR_PUPDR2;/*No pull-up, pull-down*/
    GPIOA   ->PUPDR     |= GPIO_PUPDR_PUPDR4_0;/*pull-up*/

    RCC     ->APB2ENR   |= RCC_APB2ENR_SPI1EN; /*enable clock spi1*/
    SPI1    ->CR1       |= SPI_CR1_MSTR | SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_CPHA | SPI_CR1_SSM; /* Master selection,  BR: Fpclk/32 48.000.000/32 = 1500k,CPOL = 0, CPHA = 1, enable SSM*/
    SPI1    ->CR2       |= SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_DS; /*  Slave select output enabled, RXNE IT, SPI Motorola mode, 16-bit Rx fifo  */
    SPI1    ->CR1       |= SPI_CR1_SPE; /*  Enable SPI1  */

    NVIC_EnableIRQ(SPI1_IRQn); // Enable interrupt from SPI1 (NVIC level)
}

extern "C" void SPI1_IRQHandler(void)
{
    if ((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
    {
        motor::CONTROLLER.SPI_DATA = SPI1->DR;
        motor::CONTROLLER.spiReady();
    }
}

void motor::Controller::spiReady()
{
    if (command_ == READ_ANGLE)
    {
        SET_SCS_AS5047;
        processCommand(UPDATE_CURRENT);
        command_ = NONE;
    }
    else if (command_ == CALIBRATE)
    {
        SET_SCS_AS5047;
        angleOffset_ = (SPI_DATA & 0x3FFF);
        setDuties_(0,0,0,0);
        uart::CONTROLLER.printfMessage("Calibrate DONE, angle offset:%d", angleOffset_);
        command_ = NONE;
    }
}

void motor::Controller::readAS5047_(uint16_t command)
{
    CLEAR_SCS_AS5047;
    SPI1->DR = command;
}

