#include "controller.h"
#include "stm32f0xx.h"
#include "command.h"

adc::Controller adc::CONTROLLER;

void adc::Controller::init()
{
    /*begin init led pc8 pc9*/
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0) ;
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9) ;
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8|GPIO_OSPEEDER_OSPEEDR9);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8|GPIO_PUPDR_PUPDR9);
    /*end init led pc8 pc9*/

    /*begin init ADC-DMA*/
    //ADC Clock selection
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;
    while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0){}

    //ADC enable sequence
    if ((ADC1->ISR & ADC_ISR_ADRDY) != 0)
    {
        ADC1->ISR |= ADC_ISR_ADRDY;
    }
    ADC1->CR |= ADC_CR_ADEN;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0){}

    //Single conversion sequence
    ADC1->CHSELR = ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL2 | ADC_CHSELR_CHSEL3 | ADC_CHSELR_CHSEL4;
    ADC1->SMPR |= ADC_SMPR1_SMPR_0 | ADC_SMPR1_SMPR_1 | ADC_SMPR1_SMPR_2;
    ADC->CCR |= ADC_CCR_VREFEN;

    //DMA circular mode
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
    DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
    DMA1_Channel1->CMAR = (uint32_t)(data_);
    DMA1_Channel1->CNDTR = 4;
    DMA1_Channel1->CCR |= DMA_CCR_MINC | DMA_CCR_MSIZE_0 | DMA_CCR_PSIZE_0 | DMA_CCR_TEIE | DMA_CCR_CIRC;
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    /*end init ADC-DMA*/

    /*begin init timer3*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 2399;	        // Set prescaler to 2 400 (PSC + 1) 48000000/2400 = 20.000
    TIM3->ARR = 19;	          // 1khz
    TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3->CR1 = TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
    /*end init timer3*/
}

extern "C" void TIM3_IRQHandler(void)
{
    if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
    {
        TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
        ADC1->CR |= ADC_CR_ADSTART; /* Start the ADC conversion */
    }
}

void adc::Controller::loop()
{
    if(countDelayPlot_++ > 10000)
    {
        plot::CONTROLLER.plot_pa1(data_[0]);
        plot::CONTROLLER.plot_pa2(data_[1]);
        plot::CONTROLLER.plot_pa3(data_[2]);
        plot::CONTROLLER.flush();
        countDelayPlot_ = 0;
    }
}


