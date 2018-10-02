/**
    QuangNT
 */

#if 1
#include "nrf_drv_saadc.h"
#include "nrf_drv_common.h"


#define NRF_LOG_MODULE_NAME saadc

#if SAADC_CONFIG_LOG_ENABLED
#define NRF_LOG_LEVEL       SAADC_CONFIG_LOG_LEVEL
#define NRF_LOG_INFO_COLOR  SAADC_CONFIG_INFO_COLOR
#define NRF_LOG_DEBUG_COLOR SAADC_CONFIG_DEBUG_COLOR
#define EVT_TO_STR(event)   (event == NRF_SAADC_EVENT_STARTED ? "NRF_SAADC_EVENT_STARTED" :                         \
                            (event == NRF_SAADC_EVENT_END ? "NRF_SAADC_EVENT_END" :                                 \
                            (event == NRF_SAADC_EVENT_DONE ? "NRF_SAADC_EVENT_DONE" :                               \
                            (event == NRF_SAADC_EVENT_RESULTDONE ? "NRF_SAADC_EVENT_RESULTDONE" :                   \
                            (event == NRF_SAADC_EVENT_CALIBRATEDONE ? "NRF_SAADC_EVENT_CALIBRATEDONE" :             \
                            (event == NRF_SAADC_EVENT_STOPPED ? "NRF_SAADC_EVENT_STOPPED" : "UNKNOWN EVENT"))))))
#else //SAADC_CONFIG_LOG_ENABLED
#define EVT_TO_STR(event)   ""
#define NRF_LOG_LEVEL       0
#endif //SAADC_CONFIG_LOG_ENABLED
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

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
    nrf_drv_state_t               state;                         ///< Driver initialization state.
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
    m_cb.state                = NRF_DRV_STATE_INITIALIZED;
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

/*

ret_code_t nrf_drv_saadc_channel_uninit(uint8_t channel)
{
    ASSERT(channel < NRF_SAADC_CHANNEL_COUNT)
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);

    ret_code_t err_code;

    // A channel can only be uninitialized if the driver is in the idle state.
    if (m_cb.adc_state != NRF_SAADC_STATE_IDLE)
    {
        err_code = NRF_ERROR_BUSY;
        NRF_LOG_WARNING("Function: %s, error code: %s.",
                        (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
        return err_code;
    }

    if (m_cb.psel[channel].pselp)
    {
        --m_cb.active_channels;
    }
    m_cb.psel[channel].pselp = NRF_SAADC_INPUT_DISABLED;
    m_cb.psel[channel].pseln = NRF_SAADC_INPUT_DISABLED;
    nrf_saadc_channel_input_set(channel, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);
    nrf_drv_saadc_limits_set(channel, NRF_DRV_SAADC_LIMITL_DISABLED, NRF_DRV_SAADC_LIMITH_DISABLED);
    NRF_LOG_INFO("Channel denitialized: %d.", channel);

    err_code = NRF_SUCCESS;
    NRF_LOG_INFO("Function: %s, error code: %s.",
                    (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
    return err_code;
}
*/

uint32_t nrf_drv_saadc_sample_task_get(void)
{
    return nrf_saadc_task_address_get(NRF_SAADC_TASK_SAMPLE);
    /*
    return nrf_saadc_task_address_get(
        m_cb.low_power_mode ? NRF_SAADC_TASK_START : NRF_SAADC_TASK_SAMPLE);
        */
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
/*
ret_code_t nrf_drv_saadc_sample_convert(uint8_t channel, nrf_saadc_value_t * p_value)
{
    ret_code_t err_code;

    if (m_cb.adc_state != NRF_SAADC_STATE_IDLE)
    {
        err_code = NRF_ERROR_BUSY;
        NRF_LOG_WARNING("Function: %s error code: %s.", (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
        return err_code;
    }
    m_cb.adc_state = NRF_SAADC_STATE_BUSY;
    nrf_saadc_int_disable(NRF_SAADC_INT_STARTED | NRF_SAADC_INT_END);
    nrf_saadc_buffer_init(p_value, 1);
    if (m_cb.active_channels > 1)
    {
        for (uint32_t i = 0; i < NRF_SAADC_CHANNEL_COUNT; ++i)
        {
            nrf_saadc_channel_input_set(i, NRF_SAADC_INPUT_DISABLED, NRF_SAADC_INPUT_DISABLED);
        }
    }
    nrf_saadc_channel_input_set(channel,
                                m_cb.psel[channel].pselp, m_cb.psel[channel].pseln);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);

    uint32_t timeout = HW_TIMEOUT;

    while (0 == nrf_saadc_event_check(NRF_SAADC_EVENT_END) && timeout > 0)
    {
        timeout--;
    }
    nrf_saadc_event_clear(NRF_SAADC_EVENT_STARTED);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_END);

    NRF_LOG_INFO("Conversion value: %d, channel: %d.", *p_value & 0x0000FFFF, channel);

    if (m_cb.active_channels > 1)
    {
        for (uint32_t i = 0; i < NRF_SAADC_CHANNEL_COUNT; ++i)
        {
            nrf_saadc_channel_input_set(i, m_cb.psel[i].pselp, m_cb.psel[i].pseln);
        }
    }

    if (m_cb.low_power_mode)
    {
        nrf_saadc_int_enable(NRF_SAADC_INT_STARTED | NRF_SAADC_INT_END);
    }
    else
    {
        nrf_saadc_int_enable(NRF_SAADC_INT_END);
    }

    m_cb.adc_state = NRF_SAADC_STATE_IDLE;

    err_code = NRF_SUCCESS;
    NRF_LOG_WARNING("Function: %s, error code: %s.", (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
    return err_code;
}

*/


/*

ret_code_t nrf_drv_saadc_sample()
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);

    ret_code_t err_code = NRF_SUCCESS;
    if (m_cb.adc_state != NRF_SAADC_STATE_BUSY)
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    else if (m_cb.low_power_mode)
    {
        nrf_saadc_task_trigger(NRF_SAADC_TASK_START);
    }
    else
    {
        nrf_saadc_task_trigger(NRF_SAADC_TASK_SAMPLE);
    }

    NRF_LOG_INFO("Function: %s, error code: %s.", (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
    return err_code;
}

/*
ret_code_t nrf_drv_saadc_calibrate_offset()
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);

    ret_code_t err_code;

    if (m_cb.adc_state != NRF_SAADC_STATE_IDLE)
    {
        err_code = NRF_ERROR_BUSY;
        NRF_LOG_WARNING("Function: %s, error code: %s.", (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
        return err_code;
    }

    m_cb.adc_state = NRF_SAADC_STATE_CALIBRATION;

    nrf_saadc_event_clear(NRF_SAADC_EVENT_CALIBRATEDONE);
    nrf_saadc_int_enable(NRF_SAADC_INT_CALIBRATEDONE);
    nrf_saadc_task_trigger(NRF_SAADC_TASK_CALIBRATEOFFSET);
    err_code = NRF_SUCCESS;
    NRF_LOG_INFO("Function: %s, error code: %s.", (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
    return err_code;
}
*/
/*
bool nrf_drv_saadc_is_busy(void)
{
    return (m_cb.adc_state != NRF_SAADC_STATE_IDLE);
}
*/
/*
void nrf_drv_saadc_abort(void)
{
    if (nrf_drv_saadc_is_busy())
    {
        nrf_saadc_event_clear(NRF_SAADC_EVENT_STOPPED);
        nrf_saadc_task_trigger(NRF_SAADC_TASK_STOP);

        if (m_cb.adc_state == NRF_SAADC_STATE_CALIBRATION)
        {
            m_cb.adc_state = NRF_SAADC_STATE_IDLE;
        }
        else
        {
            // Wait for ADC being stopped.
            uint32_t timeout = HW_TIMEOUT;

            while ((m_cb.adc_state != NRF_SAADC_STATE_IDLE) && (timeout > 0))
            {
                --timeout;
            }
            ASSERT(timeout > 0);
        }

        m_cb.p_buffer           = 0;
        m_cb.p_secondary_buffer = 0;
        NRF_LOG_INFO("Conversion aborted.");
    }
}

*/
/*
void nrf_drv_saadc_limits_set(uint8_t channel, int16_t limit_low, int16_t limit_high)
{
    ASSERT(m_cb.state != NRF_DRV_STATE_UNINITIALIZED);
    ASSERT(m_cb.event_handler); // only non blocking mode supported
    ASSERT(limit_low >= NRF_DRV_SAADC_LIMITL_DISABLED);
    ASSERT(limit_high <= NRF_DRV_SAADC_LIMITH_DISABLED);
    ASSERT(limit_low < limit_high);
    nrf_saadc_channel_limits_set(channel, limit_low, limit_high);

    uint32_t int_mask = nrf_saadc_limit_int_get(channel, NRF_SAADC_LIMIT_LOW);
    if (limit_low == NRF_DRV_SAADC_LIMITL_DISABLED)
    {
        m_cb.limits_enabled_flags &= ~(0x80000000 >> LOW_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_disable(int_mask);
    }
    else
    {
        m_cb.limits_enabled_flags |= (0x80000000 >> LOW_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_enable(int_mask);
    }

    int_mask = nrf_saadc_limit_int_get(channel, NRF_SAADC_LIMIT_HIGH);
    if (limit_high == NRF_DRV_SAADC_LIMITH_DISABLED)
    {
        m_cb.limits_enabled_flags &= ~(0x80000000 >> HIGH_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_disable(int_mask);
    }
    else
    {
        m_cb.limits_enabled_flags |= (0x80000000 >> HIGH_LIMIT_TO_FLAG(channel));
        nrf_saadc_int_enable(int_mask);
    }
}
*/
#endif //NRF_MODULE_ENABLED(SAADC)
