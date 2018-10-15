#ifdef MCU_NRF52

#include "nrf.h"
#include "hal/gpio.h"
#include "hal/nrf_timer.h"
#include "timer.h"
#include "nrf_drv_timer.h"
#include "hal/nrf_drv_common.h"
#include "hal/Irq_common.h"

/**@brief Timer control block. */

typedef struct
{
    nrf_timer_event_handler_t handler;
    void *                    context;
    nrf_drv_state_t           state;
} timer_control_block_t;

static timer_control_block_t m_cb[ENABLED_TIMER_COUNT];


static const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(0);


timer::Controller timer::CONTROLLER;

void timer::Controller::init(nrf_timer_event_handler_t timer_event_handler,uint32_t timer_ms)
{
    uint32_t time_ticks;
    nrf_drv_timer_config_t timer_cfg;
    timer_cfg.mode = (nrf_timer_mode_t)0;
    timer_cfg.frequency = (nrf_timer_frequency_t)NRF_TIMER_FREQ_16MHz;
    timer_cfg.bit_width = (nrf_timer_bit_width_t)3;
    timer_cfg.p_context = NULL;
    timer_cfg.interrupt_priority = 7;

    nrf_drv_timer_init(&TIMER_LED, &timer_cfg,timer_event_handler);
    //time_ticks = nrf_drv_timer_ms_to_ticks(&TIMER_LED, timer_ms);
    time_ticks = nrf_drv_timer_us_to_ticks(&TIMER_LED, timer_ms);
    nrf_drv_timer_extended_compare(&TIMER_LED, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&TIMER_LED);

}

uint32_t timer::Controller::timer_compare_event_address_get()
{
    return nrf_drv_timer_compare_event_address_get(&TIMER_LED,NRF_TIMER_CC_CHANNEL0);
}

void timer::Controller::nrf_drv_timer_init(
                                nrf_drv_timer_t const * const p_instance,
                                nrf_drv_timer_config_t const * p_config,
                                nrf_timer_event_handler_t timer_event_handler
                                )
{
    timer_control_block_t * p_cb = &m_cb[p_instance->instance_id];
    p_cb->handler = timer_event_handler;
    p_cb->context = p_config->p_context;
    uint8_t i;

    for (i = 0; i < p_instance->cc_channel_count; ++i)
    {
        nrf_timer_event_clear(p_instance->p_reg,
        nrf_timer_compare_event_get(i));
    }
    hal::IRQCOMMON.nrf_drv_common_irq_enable(nrf_drv_get_IRQn(p_instance->p_reg),p_config->interrupt_priority);

    nrf_timer_mode_set(p_instance->p_reg, p_config->mode);
    nrf_timer_bit_width_set(p_instance->p_reg, p_config->bit_width);
    nrf_timer_frequency_set(p_instance->p_reg, p_config->frequency);
}



void timer::Controller::nrf_drv_timer_enable(const nrf_drv_timer_t * const p_instance)
{
    nrf_timer_task_trigger(p_instance->p_reg, NRF_TIMER_TASK_START);
    m_cb[p_instance->instance_id].state = NRF_DRV_STATE_POWERED_ON;
}

void timer::Controller::nrf_drv_timer_extended_compare(const nrf_drv_timer_t * const p_instance, nrf_timer_cc_channel_t cc_channel, uint32_t cc_value, nrf_timer_short_mask_t timer_short_mask, bool enable_int)
{
    nrf_timer_shorts_disable(p_instance->p_reg,
        (TIMER_SHORTS_COMPARE0_STOP_Msk  << cc_channel) |
        (TIMER_SHORTS_COMPARE0_CLEAR_Msk << cc_channel));

    nrf_timer_shorts_enable(p_instance->p_reg, timer_short_mask);

    (void)nrf_drv_timer_compare(p_instance,
                                cc_channel,
                                cc_value,
                                enable_int);
}

void timer::Controller::nrf_drv_timer_compare(const nrf_drv_timer_t * const p_instance, nrf_timer_cc_channel_t cc_channel, uint32_t cc_value, bool enable_int)
{
    nrf_timer_int_mask_t timer_int = nrf_timer_compare_int_get(cc_channel);
    if (enable_int)
    {
        nrf_timer_int_enable(p_instance->p_reg, timer_int);
    }
    else
    {
        nrf_timer_int_disable(p_instance->p_reg, timer_int);
    }

    nrf_timer_cc_write(p_instance->p_reg, cc_channel, cc_value);
}

static void irq_handler(NRF_TIMER_Type * p_reg,
                        timer_control_block_t * p_cb,
                        uint8_t channel_count)
{
    uint8_t i;
    for (i = 0; i < channel_count; ++i)
    {
        nrf_timer_event_t event = nrf_timer_compare_event_get(i);
        nrf_timer_int_mask_t int_mask = nrf_timer_compare_int_get(i);

        if (nrf_timer_event_check(p_reg, event) &&
            nrf_timer_int_enable_check(p_reg, int_mask))
        {
            nrf_timer_event_clear(p_reg, event);
            p_cb->handler(event, p_cb->context);

        }
    }
}

extern "C" void TIMER0_IRQHandler(void)
{
    irq_handler(NRF_TIMER0, &m_cb[TIMER0_INSTANCE_INDEX],NRF_TIMER_CC_CHANNEL_COUNT(0));
}


#endif
