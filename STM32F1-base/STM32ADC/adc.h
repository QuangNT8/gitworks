#ifndef ADC_H
#define ADC_H

#include "stm32f0xx.h"

namespace adc
{
    class Controller
    {
    public:
        void adcReady();
        void init();
        void loop();
        int16_t adc_ch1;
        int16_t adc_ch2;
    private:
        int16_t adcBuffer_[2];
    };

extern adc::Controller CONTROLLER;

}
#endif // CONTROLLER_H
