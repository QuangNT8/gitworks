#include "stm32f0xx.h"
#include "controller.h"

void spi::Controller::initSpi_()
{
    /*begin init SPI*/
    /*
     * SPI DRV8323
     * PA4 -> SCS Serial chip select. A logic low on this pin enables serial interface communication.
     * PA5 -> SPI_CLK
     * PA6 -> SPI_SDO output DRV8323
     * PA7 -> SPI_SDI input DRV8323
     * PC4 -> DRV8323_EN Gate driver enable. Active at logic hight.
    */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOA->MODER |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER7_0);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_7);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR7);

    GPIOA->MODER &= GPIO_MODER_MODER6;/* clear pin mode to input mode */
    GPIOA->PUPDR |= GPIO_MODER_MODER6_0;/*pull up*/

    GPIOC->MODER |= (GPIO_MODER_MODER4_0) ;
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_4) ;
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4);
    /*end init SPI*/
}
