#ifndef OSC_H
#define OSC_H
#include "stdint.h"
#include "system_stm32f0xx.h"
#include "stm32f0xx_hal_def.h"
//#include "stm32f0xx_hal_rcc.h"
//#include "stm32f0xx_hal_rcc_ex.h"


namespace osc
{
    class Controller
    {
        public:
            void init();
            void SystemClock_Config(void);
            HAL_StatusTypeDef HAL_Init(void);
        private:
            void _Error_Handler(char *file, int line);

    };

extern osc::Controller CONTROLLER;

}
#endif // CONTROLLER_H
