#ifdef MCU_NRF52


#include "nrf.h"
#include "hal/gpio.h"
#include "hal/nrf_drv_common.h"
#include "hal/Irq_common.h"
#include "hal/nrf_saadc.h"
#include "nrf_drv_saadc.h"
#include "sadc.h"

// <o> SAADC_CONFIG_RESOLUTION  - Resolution
// <0=> 8 bit
// <1=> 10 bit
// <2=> 12 bit
// <3=> 14 bit
// <o> SAADC_CONFIG_OVERSAMPLE  - Sample period

// <0=> Disabled
// <1=> 2x
// <2=> 4x
// <3=> 8x
// <4=> 16x
// <5=> 32x
// <6=> 64x
// <7=> 128x
// <8=> 256x
/**
 * @brief Macro for setting @ref nrf_drv_saadc_config_t to quangnt settings.
 */
#define NRF_DRV_SAADC_QUANGNT_CONFIG                                        \
    {                                                                       \
        .resolution         = (nrf_saadc_resolution_t)2,  \
        .oversample         = (nrf_saadc_oversample_t)0,  \
        .interrupt_priority = 7,                        \
        .low_power_mode     = 0                              \
    }

sadc::Controller sadc::CONTROLLER;

typedef enum
{
    NRF_SAADC_STATE_IDLE        = 0,
    NRF_SAADC_STATE_BUSY        = 1,
    NRF_SAADC_STATE_CALIBRATION = 2
} nrf_saadc_state_t;

typedef struct
{
    nrf_saadc_input_t pselp;
    nrf_saadc_input_t pseln;
} nrf_saadc_psel_buffer;

static const nrf_drv_saadc_config_t m_default_config = NRF_DRV_SAADC_DEFAULT_CONFIG;

/** @brief SAADC control block.*/
typedef struct
{
    nrf_drv_saadc_event_handler_t event_handler;                 ///< Event handler function pointer.
    volatile nrf_saadc_value_t  * p_buffer;                      ///< Sample buffer.
    volatile uint16_t             buffer_size;                   ///< Size of the sample buffer.
    volatile nrf_saadc_value_t  * p_secondary_buffer;            ///< Secondary sample buffer.
    volatile nrf_saadc_state_t    adc_state;                     ///< State of the SAADC.
    uint32_t                      limits_enabled_flags;          ///< Enabled limits flags.
    uint16_t                      secondary_buffer_size;         ///< Size of the secondary buffer.
    uint16_t                      buffer_size_left;              ///< When low power mode is active indicates how many samples left to convert on current buffer.
    nrf_saadc_psel_buffer         psel[NRF_SAADC_CHANNEL_COUNT]; ///< Pin configurations of SAADC channels.
    uint8_t                       active_channels;               ///< Number of enabled SAADC channels.
    bool                          low_power_mode;                ///< Indicates if low power mode is active.
    bool                          conversions_end;               ///< When low power mode is active indicates end of conversions on current buffer.
} nrf_drv_saadc_cb_t;

static nrf_drv_saadc_cb_t m_cb;


#define LOW_LIMIT_TO_FLAG(channel)      ((2 * channel + 1))
#define HIGH_LIMIT_TO_FLAG(channel)     ((2 * channel))
#define FLAG_IDX_TO_EVENT(idx)          ((nrf_saadc_event_t)((uint32_t)NRF_SAADC_EVENT_CH0_LIMITH + \
                                            4 * idx))
#define LIMIT_EVENT_TO_CHANNEL(event)   (uint8_t)(((uint32_t)event - \
                                            (uint32_t)NRF_SAADC_EVENT_CH0_LIMITH) / 8)
#define LIMIT_EVENT_TO_LIMIT_TYPE(event)((((uint32_t)event - (uint32_t)NRF_SAADC_EVENT_CH0_LIMITH) & 4) \
                                            ? NRF_SAADC_LIMIT_LOW : NRF_SAADC_LIMIT_HIGH)
#define HW_TIMEOUT 10000

#define SAMPLES_IN_BUFFER 4
static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];




void sadc::Controller::init(nrf_drv_saadc_event_handler_t saadc_callback)
{
    nrf_drv_saadc_init(m_buffer_pool[0],SAMPLES_IN_BUFFER, saadc_callback);
}


void sadc::Controller::nrf_drv_saadc_init(    nrf_saadc_value_t * p_buffer,
                                              uint16_t size,
                                              nrf_drv_saadc_event_handler_t  event_handler)
{
    nrf_drv_saadc_config_t const * p_config;
    nrf_drv_saadc_config_t m_QUANGNT_config = NRF_DRV_SAADC_QUANGNT_CONFIG;
    p_config = &m_QUANGNT_config;

    m_cb.event_handler = event_handler;
    nrf_saadc_resolution_set(p_config->resolution);
    nrf_saadc_oversample_set(p_config->oversample);

    m_cb.low_power_mode       = p_config->low_power_mode;
    m_cb.adc_state            = NRF_SAADC_STATE_IDLE;
    m_cb.active_channels      = 0;
    m_cb.limits_enabled_flags = 0;
    m_cb.conversions_end      = false;

    nrf_saadc_int_disable(NRF_SAADC_INT_ALL);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);

    hal::IRQCOMMON.nrf_drv_common_irq_enable(SAADC_IRQn, p_config->interrupt_priority);
    nrf_saadc_int_enable(NRF_SAADC_INT_END);

    nrf_saadc_channel_config_t channel_0_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
    channel_0_config.gain = NRF_SAADC_GAIN1_4;
    channel_0_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_channel_config_t channel_1_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);
    channel_1_config.gain = NRF_SAADC_GAIN1_4;
    channel_1_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_channel_config_t channel_2_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN6);
    channel_2_config.gain = NRF_SAADC_GAIN1_4;
    channel_2_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_saadc_channel_config_t channel_3_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN7);

    channel_3_config.gain = NRF_SAADC_GAIN1_4;
    channel_3_config.reference = NRF_SAADC_REFERENCE_VDD4;

    nrf_drv_saadc_channel_init(0,&channel_0_config);
    nrf_drv_saadc_channel_init(1,&channel_1_config);
    nrf_drv_saadc_channel_init(2,&channel_2_config);
    nrf_drv_saadc_channel_init(3,&channel_3_config);

    nrf_drv_saadc_buffer_convert( p_buffer, size);

    nrf_saadc_enable();
}

uint32_t sadc::Controller::sample_task_get()
{
    return nrf_saadc_task_address_get(NRF_SAADC_TASK_SAMPLE);
}

void sadc::Controller::nrf_drv_saadc_channel_init(uint8_t channel, const nrf_saadc_channel_config_t * const p_config)
{
    if (!m_cb.psel[channel].pselp)
    {
        ++m_cb.active_channels;
    }
    m_cb.psel[channel].pselp = p_config->pin_p;
    m_cb.psel[channel].pseln = p_config->pin_n;
    nrf_saadc_channel_init(channel, p_config);
    nrf_saadc_channel_input_set(channel, p_config->pin_p, p_config->pin_n);
}

void sadc::Controller::nrf_saadc_channel_init(uint8_t channel, const nrf_saadc_channel_config_t * const config)
{
    NRF_SAADC->CH[channel].CONFIG =
            ((config->resistor_p   << SAADC_CH_CONFIG_RESP_Pos)   & SAADC_CH_CONFIG_RESP_Msk)
            | ((config->resistor_n << SAADC_CH_CONFIG_RESN_Pos)   & SAADC_CH_CONFIG_RESN_Msk)
            | ((config->gain       << SAADC_CH_CONFIG_GAIN_Pos)   & SAADC_CH_CONFIG_GAIN_Msk)
            | ((config->reference  << SAADC_CH_CONFIG_REFSEL_Pos) & SAADC_CH_CONFIG_REFSEL_Msk)
            | ((config->acq_time   << SAADC_CH_CONFIG_TACQ_Pos)   & SAADC_CH_CONFIG_TACQ_Msk)
            | ((config->mode       << SAADC_CH_CONFIG_MODE_Pos)   & SAADC_CH_CONFIG_MODE_Msk)
            | ((config->burst      << SAADC_CH_CONFIG_BURST_Pos)  & SAADC_CH_CONFIG_BURST_Msk);
    nrf_saadc_channel_input_set(channel, config->pin_p, config->pin_n);
}

void sadc::Controller::nrf_drv_saadc_buffer_convert(nrf_saadc_value_t *p_buffer, uint16_t size)
{
    nrf_saadc_int_enable(NRF_SAADC_INT_END);
    m_cb.p_buffer           = p_buffer;
    m_cb.buffer_size        = size;
    m_cb.p_secondary_buffer = NULL;

    nrf_saadc_buffer_init(p_buffer, size);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
}

#if 1
extern "C" void SAADC_IRQHandler(void)
{
    if (nrf_saadc_event_check(NRF_SAADC_EVENT_END))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
        nrf_drv_saadc_evt_t evt;
        evt.type               = NRF_DRV_SAADC_EVT_DONE;
        evt.data.done.p_buffer = (nrf_saadc_value_t *)m_cb.p_buffer;
        evt.data.done.size     = m_cb.buffer_size;

        if (m_cb.p_secondary_buffer == NULL)
        {
            m_cb.adc_state = NRF_SAADC_STATE_IDLE;
        }
        else
        {
            m_cb.buffer_size_left   = m_cb.secondary_buffer_size;
            m_cb.p_buffer           = m_cb.p_secondary_buffer;
            m_cb.buffer_size        = m_cb.secondary_buffer_size;
            m_cb.p_secondary_buffer = NULL;
            if (!m_cb.low_power_mode)
            {
                nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
            }
        }
        m_cb.event_handler(&evt);
        m_cb.conversions_end = false;

    }
}
#endif
#if 0
extern "C" void SAADC_IRQHandler(void)
{
    if (nrf_saadc_event_check(NRF_SAADC_EVENT_END))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_END);

        if (!m_cb.low_power_mode || m_cb.conversions_end)
        {
            nrf_drv_saadc_evt_t evt;
            evt.type               = NRF_DRV_SAADC_EVT_DONE;
            evt.data.done.p_buffer = (nrf_saadc_value_t *)m_cb.p_buffer;
            evt.data.done.size     = m_cb.buffer_size;

            if (m_cb.p_secondary_buffer == NULL)
            {
                m_cb.adc_state = NRF_SAADC_STATE_IDLE;
            }
            else
            {
                m_cb.buffer_size_left   = m_cb.secondary_buffer_size;
                m_cb.p_buffer           = m_cb.p_secondary_buffer;
                m_cb.buffer_size        = m_cb.secondary_buffer_size;
                m_cb.p_secondary_buffer = NULL;
                if (!m_cb.low_power_mode)
                {
                    nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
                }
            }
            m_cb.event_handler(&evt);
            m_cb.conversions_end = false;
        }
    }
    if (m_cb.low_power_mode && nrf_saadc_event_check(NRF_SAADC_EVENT_STARTED))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);

        if (m_cb.buffer_size_left > m_cb.active_channels)
        {
            // More samples to convert than for single event.
            m_cb.buffer_size_left -= m_cb.active_channels;
            nrf_saadc_buffer_init((nrf_saadc_value_t *)&m_cb.p_buffer[m_cb.buffer_size -
                                                                      m_cb.buffer_size_left],
                                  m_cb.active_channels);
        }
        else if ((m_cb.buffer_size_left == m_cb.active_channels) &&

                 (m_cb.p_secondary_buffer != NULL))
        {
            // Samples to convert for one event, prepare next buffer.
            m_cb.conversions_end  = true;
            m_cb.buffer_size_left = 0;
            nrf_saadc_buffer_init((nrf_saadc_value_t *)m_cb.p_secondary_buffer,
                                  m_cb.active_channels);
        }
        else if (m_cb.buffer_size_left == m_cb.active_channels)
        {
            // Samples to convert for one event, but no second buffer.
            m_cb.conversions_end  = true;
            m_cb.buffer_size_left = 0;
        }
        nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
        nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);
    }
    if (nrf_saadc_event_check(NRF_SAADC_EVENT_CALIBRATEDONE))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_CALIBRATEDONE);
        m_cb.adc_state = NRF_SAADC_STATE_IDLE;

        nrf_drv_saadc_evt_t evt;
        evt.type = NRF_DRV_SAADC_EVT_CALIBRATEDONE;
        m_cb.event_handler(&evt);
    }
    if (nrf_saadc_event_check(NRF_SAADC_EVENT_STOPPED))
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_STOPPED);
        m_cb.adc_state = NRF_SAADC_STATE_IDLE;
    }
    else
    {
        uint32_t          limit_flags = m_cb.limits_enabled_flags;
        uint32_t          flag_idx;
        nrf_saadc_event_t event;

        while (limit_flags)
        {
            flag_idx     = __CLZ(limit_flags);
            limit_flags &= ~((1UL << 31) >> flag_idx);
            event        = FLAG_IDX_TO_EVENT(flag_idx);
            if (nrf_saadc_event_check(event))
            {
                nrf_saadc_event_clear(event);
                nrf_drv_saadc_evt_t evt;
                evt.type                  = NRF_DRV_SAADC_EVT_LIMIT;
                evt.data.limit.channel    = LIMIT_EVENT_TO_CHANNEL(event);
                evt.data.limit.limit_type = LIMIT_EVENT_TO_LIMIT_TYPE(event);
                m_cb.event_handler(&evt);
            }
        }
    }
}
#endif
#endif

