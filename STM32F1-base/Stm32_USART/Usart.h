#ifndef USART_H
#define USART_H
#include "stm32f103xb.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_tim.h"



namespace USART
{
    class Controller
    {
    public:
        void init();
        void loop();
        void myprintf(const char *fmt,...);
    private:
        void vprint(const char *fmt, char * argp);
    };

extern USART::Controller CONTROLLER;

}
#endif // CONTROLLER_H
