#include "controller.h"
#include "uart/controller.h"
#include "stm32f0xx.h"

void foc::Controller::initGPIO_()
{    
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    /*
     * LED
     * PA15 -> LED
    */
    GPIOA->MODER |= (GPIO_MODER_MODER15_0);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_15);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR15);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR15);

    /*
     * PWM-TIMER1-6CH
     * PA8  -> TIM1-CH1
     * PA9  -> TIM1-CH2
     * PA10 -> TIM1-CH3
     * PB13 -> TIM1-CH1N
     * PB14 -> TIM1-CH2N
     * PB15 -> TIM1-CH3N
    */
    GPIOA->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);                 /*begin pwm*/
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9  | GPIO_PUPDR_PUPDR10);
    GPIOA->AFR[1] = 0x00000222;//AF2

    GPIOB->MODER |= (GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_14 | GPIO_OTYPER_OT_15);
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13 | GPIO_PUPDR_PUPDR14  | GPIO_PUPDR_PUPDR15);                   /*end pwm*/
    GPIOB->AFR[1] = 0x22200000;//AF2

    /*
     * SPI DRV8323
     * PA4 -> SCS Serial chip select. A logic low on this pin enables serial interface communication.
     * PA5 -> SPI_CLK
     * PA6 -> SPI_SDO output DRV8323
     * PA7 -> SPI_SDI input DRV8323
     * PC4 -> DRV8323_EN Gate driver enable. Active at logic hight.
    */
    GPIOA->MODER |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5  | GPIO_PUPDR_PUPDR6 | GPIO_PUPDR_PUPDR7);

    GPIOC->MODER |= (GPIO_MODER_MODER4_0) ;
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_4) ;
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4);

    /*
     * ENCODER-TIMER2
     * PA0 -> Encoder ch1
     * PA1 -> Encoder ch2
     * PA15 -> Led
    */
    GPIOA->MODER |= (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_0 | GPIO_OTYPER_OT_1);
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1);
    GPIOA->AFR[0] |= 34;
}

void foc::Controller::initPWM_()
{
    /*begin init timer1*/
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->PSC = 0;
    TIM1->ARR = 1199;
    TIM1->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM1->CR1 = TIM_CR1_CEN;   // Enable timer

    /* set pwm1 mode (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    /* set pwm1 mode (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 3 */
    TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;

    /* Enable channel outputs */
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1NE;
    TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM1->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3NE;

    /* output idle state HIGHT */
    TIM1->CR2 |= TIM_CR2_OIS1 | TIM_CR2_OIS1N;
    TIM1->CR2 |= TIM_CR2_OIS2 | TIM_CR2_OIS2N;
    TIM1->CR2 |= TIM_CR2_OIS3 | TIM_CR2_OIS3N;

    /* active LOW */
    TIM1->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
    TIM1->CCER |= TIM_CCER_CC2P | TIM_CCER_CC2NP;
    TIM1->CCER |= TIM_CCER_CC3P | TIM_CCER_CC3NP;

    /*dead time 0-255*/
    /*
     *DTG[7:5]=0xx => DT=DTG[7:0]x tdtg with tdtg=tDTS.
     *DTG[7:5]=10x => DT=(64+DTG[5:0])xtdtg with Tdtg=2xtDTS.
     *DTG[7:5]=110 => DT=(32+DTG[4:0])xtdtg with Tdtg=8xtDTS.
     *DTG[7:5]=111 => DT=(32+DTG[4:0])xtdtg with Tdtg=16xtDTS.
     *Example if TDTS=125 ns (8 MHz), dead-time possible values are:
     *0 to 15875 ns by 125 ns steps,
     *16 us to 31750 ns by 250 ns steps,
     *32 us to 63 us by 1 us steps,
     *64 us to 126 us by 2 us steps
    */
    TIM1->BDTR |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select center-aligned mode 1 (CMS = 01) */
    /* (8) Force update generation (UG = 1) */
    TIM1->BDTR |= TIM_BDTR_MOE; /* (6) */
    TIM1->CR1 |= TIM_CR1_CMS_0 | TIM_CR1_CEN; /* (7) */
    TIM1->EGR |= TIM_EGR_UG; /* (8) */
    /*end init timer1*/
}

void foc::Controller::initADC_()
{
    /*ADC Clock selection*/
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0){}

    /*ADC enable sequence*/
    if ((ADC1->ISR & ADC_ISR_ADRDY) != 0)
    {
        ADC1->ISR |= ADC_ISR_ADRDY;
    }
    ADC1->CR |= ADC_CR_ADEN;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0){}

    /*
     * Single conversion sequence
     * ADC_CHSELR_CHSEL2  -> Reserve    (readADC[0])(PA2)
     * ADC_CHSELR_CHSEL10 -> IA         (readADC[1])(PC0)
     * ADC_CHSELR_CHSEL11 -> IB         (readADC[2])(PC1)
     * ADC_CHSELR_CHSEL12 -> IC         (readADC[3])(PC2)
     * ADC_CHSELR_CHSEL13 -> PVDD       (readADC[4])(PC3)
    */
    ADC1->CHSELR = ADC_CHSELR_CHSEL2 | ADC_CHSELR_CHSEL10 | ADC_CHSELR_CHSEL11 | ADC_CHSELR_CHSEL12 | ADC_CHSELR_CHSEL13;
    ADC1->SMPR |= ADC_SMPR1_SMPR_0 | ADC_SMPR1_SMPR_1 | ADC_SMPR1_SMPR_2;
    ADC->CCR |= ADC_CCR_VREFEN;

    /*DMA circular mode*/
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
    DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t)(readAdc_);
    DMA1_Channel1->CNDTR = 5;
    DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_TEIE | DMA_CCR_CIRC;
    DMA1_Channel1->CCR |= DMA_CCR_EN;
}

void foc::Controller::initTimer3_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 2399;
    TIM3->ARR = 19;	          // 1khz
    TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
}

void foc::Controller::initSPI_()
{
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; /*enable clock spi1*/
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR | SPI_CR1_CPHA | SPI_CR1_SSM; /* Master selection,  BR: Fpclk/256 48.000.000/256 = 185.5k,CPOL = 0, CPHA = 1, enable SSM*/
    SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_RXNEIE | SPI_CR2_FRF | SPI_CR2_DS; /*  Slave select output enabled, RXNE IT, SPI Motorola mode, 16-bit Rx fifo  */
    SPI1->CR1 |= SPI_CR1_SPE; /*  Enable SPI1  */
}

void foc::Controller::setupSPI_()
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
    /*set DRV8323_EN (PC4)*/
    GPIOC->BSRR = GPIO_BSRR_BS_4;
    delay_(1000);
    drv8323Write_(ADR_DRV_CTRL, DRV8323regDrvCtrl_);
    delay_(1000);
    drv8323Write_(ADR_GATE_DRV_HS, DRV8323regGateDrvHS_);
    delay_(1000);
    drv8323Write_(ADR_GATE_DRV_LS, DRV8323regGateDrvLS_);
    delay_(1000);
    drv8323Write_(ADR_OCP_CTRL, DRV8323regOcpCtrl_);
    delay_(1000);
    drv8323Write_(ADR_CSA_CTRL, DRV8323regCsaCtrl_);
    delay_(1000);
}

void foc::Controller::drv8323Write_(uint8_t addr,uint16_t data)
{
    uint16_t controlword_ = ((addr & 0x7) << 11) | (data & 0x7ff); //MSbit =0 for write, address is 3 bits (MSbit is always 0), data is 11 bits
    flagWriteSpi_ = 1;
    if ((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)
    {
        *(uint16_t *)&(SPI1->DR) = controlword_;
    }
    uint16_t temp_ = SPI1->DR;
}

void foc::Controller::drv8323Read_()
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

void foc::Controller::delay_(uint32_t delay)
{
    volatile int32_t i, j;
    for (i=0 ; i < delay; i++){j++;}
}

void foc::Controller::initEncoderTimer2_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 0;
    TIM2->ARR = 2399;
    TIM2->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)

    /* (1) Configure TI1FP1 on TI1 (CC1S = 01), configure TI1FP2 on TI2 (CC2S = 01) */
    /* (2) Configure TI1FP1 and TI1FP2 non inverted (CC1P = CC2P = 0, reset value) */
    /* (3) Configure both inputs are active on both rising and falling edges (SMS = 011) */
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0; /* (1)*/
    TIM2->CCER &= (uint16_t)(~(TIM_CCER_CC1P | TIM_CCER_CC2P)); /* (2) */
    TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; /* (3) */
    TIM2->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt from TIM2 (NVIC level)
}

void foc::Controller::setDuties_(uint16_t pwmA, uint16_t pwmB, uint16_t pwmC)
{
    if (!motorEnabale_) pwmA = pwmB = pwmC = 0;
    TIM1->CCR1 = pwmA;
    TIM1->CCR2 = pwmB;
    TIM1->CCR3 = pwmC;
}

uint16_t foc::Controller::readEncoder_()
{
    return(TIM2->CNT);
}

void foc::Controller::resetEncoder_()
{
    TIM2->CNT = 0;
}

void foc::Controller::delayTimer3()
{
    if(flag2sAlignMotor_)
    {
        cnt2sAlignMotor_++;
    }
}

void foc::Controller::startAdcconv()
{
    ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
}

extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
        foc::CONTROLLER.trigger();
        foc::CONTROLLER.delayTimer3();
    }
}

extern "C" void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF; // clear UIF flag
    }
}
