#include "nrf.h"
#include "Irq_common.h"

hal::IRQcommon hal::IRQCOMMON;

void hal::IRQcommon::nrf_drv_common_irq_enable(IRQn_Type IRQn, uint8_t priority)
{
    NVIC_SetPriority(IRQn, priority);
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_EnableIRQ(IRQn);
}
