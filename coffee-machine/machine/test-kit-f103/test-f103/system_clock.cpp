#include "controller.h"


void kit::Controller::initSystem()
{
#if 0//Clock ben trong max speed 36Mhz system
    //RCC->CR
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY)){;}// check flag HSIRDY = 1

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1

    //RCC->CFGR
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY){;}// check flag PLLRDY = 0

    RCC->CFGR &= ~RCC_CFGR_PLLSRC;//0: HSI oscillator clock / 2 selected as PLL input clock
    RCC->CFGR |= RCC_CFGR_PLLMULL9;//nhan tan trong PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;// clock PLL lam dau vao cho Clock system mux
    while ((RCC->CR & RCC_CFGR_SWS_PLL)){;}
    //he so chia AHB prescaler = 0 la chia 1

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1
#endif

#if 1//Clock ben ngoai max speed 72Mhz system
    /*RCC->CR*/
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY)){;}// check flag HSIRDY = 1

    /* enable flash prefetch buffer */
    FLASH->ACR |= FLASH_ACR_LATENCY_1;
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1

    /*RCC->CFGR*/
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY){;}// check flag PLLRDY = 0

    RCC->CFGR |= RCC_CFGR_PLLSRC;//1: Clock from PREDIV1 selected as PLL input clock
    RCC->CFGR |= RCC_CFGR_PLLMULL9;//nhan tan trong PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;// clock PLL lam dau vao cho Clock system mux
    while ((RCC->CR & RCC_CFGR_SWS_PLL)){;}
    //he so chia AHB prescaler = 0 la chia 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;//100: HCLK divided by 2

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1
#endif
}
