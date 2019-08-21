#ifndef HAL_IRQcommon_H
#define HAL_IRQcommon_H

#include "nrf.h"

namespace hal {

    class IRQcommon
    {
        public:
            void nrf_drv_common_irq_enable(IRQn_Type IRQn, uint8_t priority);
    };
    extern hal::IRQcommon IRQCOMMON;
}


#endif // HAL_IRQcommon_H
