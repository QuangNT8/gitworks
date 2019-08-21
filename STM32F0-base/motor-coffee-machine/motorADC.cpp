#include "motor.h"

void step::Motor::initADC_()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    /*********************************
    * ADC
    * ADC_CHSELR_CHSEL10 -> IA         (readADC[1])(PC0)
    * ADC_CHSELR_CHSEL11 -> IB         (readADC[2])(PC1)
    * ********************************
    */
    GPIOC->MODER |= (GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

    /*ADC Clock selection*/
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;/*HSI14*/
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
    */
    ADC1->CHSELR = ADC_CHSELR_CHSEL10 | ADC_CHSELR_CHSEL11;
    ADC1->SMPR |= ADC_SMPR1_SMPR_0;/*001 7.5 ADC clock cycles (7.5*1/14 = 0.54us <=> 1 chanel)*/
    ADC1->CFGR1 &= ~ADC_CFGR1_RES; /*00: 12 bits ADC conversion time: 1.0 µs for 12-bit resolution  <=> 1 chanel)*/

    /*DMA circular mode*/
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
    DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t)(adcBuffer_);
    DMA1_Channel1->CNDTR = 2;
    DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_CIRC | DMA_CCR_TCIE;
    DMA1_Channel1->CCR |= DMA_CCR_EN;

    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(DMA1_Channel1_IRQn,1);

    /*begin test*/
    ADC1->CFGR1 |= ADC_CFGR1_EXTEN_0;/*01: Hardware trigger detection on the rising edge*/
    ADC1->CFGR1 &= ~(ADC_CFGR1_EXTSEL_0 | ADC_CFGR1_EXTSEL_1 | ADC_CFGR1_EXTSEL_2);/*000 TRG0 TIM1_TRGO*/
    ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
    /*end test*/
}

extern "C" void DMA1_Channel1_IRQHandler(void) //40kHz
{
    if( (DMA1->ISR & DMA_ISR_TCIF1) == DMA_ISR_TCIF1  )
    {
        DMA1->IFCR  |= DMA_IFCR_CTCIF1;
        step::MOTOR.adcReady();
    }
}

#define FILTER_SHIFT    3
void step::Motor::adcReady()
{
    ia_ = adcBuffer_[0] - CALIBRATE_ADC;
    ib_ = adcBuffer_[1] - CALIBRATE_ADC;

    iA_= (AMPE_ZERO*adcBuffer_[0])/CALIBRATE_ADC;
    iA_ = 1000*(iA_ - AMPE_ZERO)/ACS712_5A_MV_A;
    iB_= (AMPE_ZERO*adcBuffer_[1])/CALIBRATE_ADC;
    iB_ = 1000*(iB_ - AMPE_ZERO)/ACS712_5A_MV_A;
}
