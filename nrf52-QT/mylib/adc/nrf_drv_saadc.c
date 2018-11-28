/**
    QuangNT
 */

#include "nrf_drv_saadc.h"

/**
 * @brief Macro for setting @ref nrf_drv_saadc_config_t to quangnt settings.
 */
#define NRF_DRV_SAADC_QUANGNT_CONFIG                                        \
    {                                                                       \
    .resolution         = (nrf_saadc_resolution_t)NRF_SAADC_RESOLUTION_12BIT,  \
    .oversample         = (nrf_saadc_oversample_t)SAADC_CONFIG_OVERSAMPLE,  \
    .interrupt_priority = SAADC_CONFIG_IRQ_PRIORITY,                        \
    .low_power_mode     = 0                              \
    }



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



void SAADC_IRQHandler(void)
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


void nrf_drv_saadc_init(    nrf_saadc_value_t * p_buffer,
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
    //m_cb.state                = NRF_DRV_STATE_INITIALIZED;
    m_cb.adc_state            = NRF_SAADC_STATE_IDLE;
    m_cb.active_channels      = 0;
    m_cb.limits_enabled_flags = 0;
    m_cb.conversions_end      = false;

    nrf_saadc_int_disable(NRF_SAADC_INT_ALL);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_END);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);
    nrf_drv_common_irq_enable(SAADC_IRQn, p_config->interrupt_priority);
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


void nrf_drv_saadc_channel_init(uint8_t                                  channel,
                                      nrf_saadc_channel_config_t const * const p_config)
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



uint32_t nrf_drv_saadc_sample_task_get(void)
{
    return nrf_saadc_task_address_get(NRF_SAADC_TASK_SAMPLE);
}

void nrf_drv_saadc_buffer_convert(nrf_saadc_value_t * p_buffer, uint16_t size)
{
    nrf_saadc_int_enable(NRF_SAADC_INT_END);

    m_cb.p_buffer           = p_buffer;
    m_cb.buffer_size        = size;
    m_cb.p_secondary_buffer = NULL;

    nrf_saadc_buffer_init(p_buffer, size);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_START);

}
