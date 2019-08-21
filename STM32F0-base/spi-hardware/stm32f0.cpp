#include "stm32f0xx.h"
#include "spihardware.h"
#include "command.h"
#include "uart/controller.h"

spi::settup spi::SETTUP;

void spi::settup::init()
{
    DRV8323regDrvCtrl_ =
      0 << 9  | //DIS_CPUV
      1 << 8  | //DIS_GDF
      1 << 7  | //OTW_REP
      0 << 5  | //PWM_MODE
      0 << 4  | //1PWM_COM
      0 << 3  | //1PWM_DIR
      0 << 2  | //COAST
      0 << 1  | //BRAKE
      0;        //CLR_FLT

    DRV8323regGateDrvHS_ =
      3 << 8  | //LOCK
      11 << 4 | //IDRIVEP_HS
      15;       //IDRIVEN_HS

    DRV8323regGateDrvLS_ =
      1 << 10 | //CBC
      2 << 8  | //TDRIVE
      15 << 4 | //IDRIVEP_LS
      15;       //IDRIVEN_LS

    DRV8323regOcpCtrl_ =
      0 << 10 | //TRETRY
      1 << 8  | //DEAD_TIME
      1 << 6  | //OCP_MODE
      2 << 4  | //OCP_DEG
      0;        //VDS_LVL

    DRV8323regCsaCtrl_ =
      0 << 10 | //CSA_FET
      1 << 9  | //VREF_DIV
      0 << 8  | //LS_REF
      2 << 6  | //CSA_GAIN
      0 << 5  | //DIS_SEN
      0 << 4  | //CSA_CAL_A
      0 << 3  | //CCSA_CAL_B
      0 << 2  | //CCSA_CAL_C
      3;

    initGpio_();
    DRV8323_setup_();
}

void spi::settup::initGpio_()
{
    /*
     * SPI DRV8323
     * PA4 -> SCS Serial chip select. A logic low on this pin enables serial interface communication.
     * PA5 -> SPI_CLK
     * PA6 -> SPI_SDO output DRV8323
     * PA7 -> SPI_SDI input DRV8323
     * PC4 -> DRV8323_EN Gate driver enable. Active at logic hight.
     * PA15 -> Led
    */

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOA->MODER |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER15_0);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_15);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR15);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5  | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR15);

    GPIOC->MODER |= (GPIO_MODER_MODER4_0) ;
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_4) ;
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4);

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; /*enable clock spi1*/
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR | SPI_CR1_CPHA | SPI_CR1_SSM; /* Master selection,  BR: Fpclk/256 48.000.000/256 = 185.5k,CPOL = 0, CPHA = 1, enable SSM*/
    SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_FRF | SPI_CR2_DS; /*  Slave select output enabled, RXNE IT, SPI Motorola mode, 16-bit Rx fifo  */
    SPI1->CR1 |= SPI_CR1_SPE; /*  Enable SPI1  */
}

void spi::settup::DRV8323_setup_()
{
    DRV8323_en_();
    delay_(1000);
    DRV8323_write_(ADR_DRV_CTRL, DRV8323regDrvCtrl_);
    delay_(1000);
    DRV8323_write_(ADR_GATE_DRV_HS, DRV8323regGateDrvHS_);
    delay_(1000);
    DRV8323_write_(ADR_GATE_DRV_LS, DRV8323regGateDrvLS_);
    delay_(1000);
    DRV8323_write_(ADR_OCP_CTRL, DRV8323regOcpCtrl_);
    delay_(1000);
    DRV8323_write_(ADR_CSA_CTRL, DRV8323regCsaCtrl_);
    delay_(1000);
}

void spi::settup::DRV8323_en_()
{
    GPIOC->BSRR = GPIO_BSRR_BS_4;/*set DRV8323_EN (PC4)*/
    delay_(1000);
}


void spi::settup::loop()
{
    DRV8323_read_();
    if(countDelayPlot_++ > 30000)
    {
        uart::CONTROLLER.printfMessage("regSpi_ = %d, %d, %d, %d, %d, %d, %d, %d",regSpi_[0], regSpi_[1], regSpi_[2], regSpi_[3], regSpi_[4], regSpi_[5], regSpi_[6], countTest_);
        countDelayPlot_ = 0;
        countTest_++;
    }
}


void spi::settup::DRV8323_read_()
{    
    uint16_t controlword_ = 0x8000 | ((countRead_ & 0x7) << 11); //MSbit =1 for read, address is 3 bits (MSbit is always 0), data is 11 bits

    if (((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE) && (flagRxSpi_ == 0))
    {
        *(uint16_t *)&(SPI1->DR) = controlword_;
        flagRxSpi_ = 1;
    }

    if((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
    {
        if(flagWriteSpi_ == 1)
        {
            flagWriteSpi_ = 0;
            uint16_t temp_ = SPI1->DR;
        }
        else
        {
            regSpi_[countRead_] = (SPI1->DR) & 0x07ff;
            flagRxSpi_ = 0;
        }
    }

    if(flagRxSpi_ == 0)
    {
        countRead_++;
        if(countRead_ >= 7) countRead_ = 0;
    }
}

void spi::settup::DRV8323_write_(int8_t addr,int16_t dataT)
{
    uint16_t controlword_ = ((addr & 0x7) << 11) | (dataT & 0x7ff); //MSbit =0 for write, address is 3 bits (MSbit is always 0), data is 11 bits
    flagWriteSpi_ = 1;
    if ((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)
    {
        *(uint16_t *)&(SPI1->DR) = controlword_;
    }
    uint16_t temp_ = SPI1->DR;
}

void spi::settup::delay_(int32_t delay)
{
    volatile int32_t i, j;
    for (i=0 ; i < delay; i++){j++;}
}
