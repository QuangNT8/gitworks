#ifndef TIMER_H
#define TIMER_H
#include "nrf_drv_timer.h"
#include "hal/Irq_common.h"
#define TIMER0_CC_NUM 4
#define TIMER1_CC_NUM 4
#define TIMER2_CC_NUM 4
#define TIMER3_CC_NUM 6
#define TIMER4_CC_NUM 6

namespace timer
{
    class Controller
    {
    public:
        void init(nrf_timer_event_handler_t timer_event_handler,uint32_t timer_ms);
        void loop();
        uint32_t timer_compare_event_address_get(void);
    private:
        void nrf_drv_timer_init(  nrf_drv_timer_t const * const p_instance,
                    nrf_drv_timer_config_t const * p_config,
                    nrf_timer_event_handler_t timer_event_handler );
        void nrf_drv_timer_enable(nrf_drv_timer_t const * const p_instance);
        void nrf_drv_timer_extended_compare(nrf_drv_timer_t const * const p_instance,
                                            nrf_timer_cc_channel_t cc_channel,
                                            uint32_t               cc_value,
                                            nrf_timer_short_mask_t timer_short_mask,
                                            bool                   enable_int);
        void nrf_drv_timer_compare(nrf_drv_timer_t const * const p_instance,
                                   nrf_timer_cc_channel_t cc_channel,
                                   uint32_t               cc_value,
                                   bool                   enable_int);
    };

extern timer::Controller CONTROLLER;

}
#endif // CONTROLLER_H
