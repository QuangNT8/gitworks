/*QuangNT*/
#include "nrf_saadc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Macro for setting @ref nrf_drv_saadc_config_t to default settings.
 */
#define NRF_DRV_SAADC_DEFAULT_CONFIG                                        \
    {                                                                       \
    .resolution         = (nrf_saadc_resolution_t)SAADC_CONFIG_RESOLUTION,  \
    .oversample         = (nrf_saadc_oversample_t)SAADC_CONFIG_OVERSAMPLE,  \
    .interrupt_priority = SAADC_CONFIG_IRQ_PRIORITY,                        \
    .low_power_mode     = SAADC_CONFIG_LP_MODE                              \
    }


/**
 * @brief Macro for setting @ref nrf_saadc_channel_config_t to default settings
 *        in single ended mode.
 *
 * @param PIN_P Analog input.
 */
#define NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(PIN_P) \
    {                                                  \
        .resistor_p = NRF_SAADC_RESISTOR_DISABLED,     \
        .resistor_n = NRF_SAADC_RESISTOR_DISABLED,     \
        .gain       = NRF_SAADC_GAIN1_6,               \
        .reference  = NRF_SAADC_REFERENCE_INTERNAL,    \
        .acq_time   = NRF_SAADC_ACQTIME_10US,          \
        .mode       = NRF_SAADC_MODE_SINGLE_ENDED,     \
        .burst      = NRF_SAADC_BURST_DISABLED,        \
        .pin_p      = (nrf_saadc_input_t)(PIN_P),      \
        .pin_n      = NRF_SAADC_INPUT_DISABLED         \
    }

/**
 * @brief Macro for setting @ref nrf_saadc_channel_config_t to default settings
 *        in differential mode.
 *
 * @param PIN_P Positive analog input.
 * @param PIN_N Negative analog input.
 */
#define NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_DIFFERENTIAL(PIN_P, PIN_N) \
    {                                                                   \
        .resistor_p = NRF_SAADC_RESISTOR_DISABLED,                      \
        .resistor_n = NRF_SAADC_RESISTOR_DISABLED,                      \
        .gain       = NRF_SAADC_GAIN1_6,                                \
        .reference  = NRF_SAADC_REFERENCE_INTERNAL,                     \
        .acq_time   = NRF_SAADC_ACQTIME_10US,                           \
        .mode       = NRF_SAADC_MODE_DIFFERENTIAL,                      \
        .pin_p      = (nrf_saadc_input_t)(PIN_P),                       \
        .pin_n      = (nrf_saadc_input_t)(PIN_N)                        \
    }

/**
 * @brief Analog-to-digital converter driver configuration structure.
 */
typedef struct
{
    nrf_saadc_resolution_t resolution;         ///< Resolution configuration.
    nrf_saadc_oversample_t oversample;         ///< Oversampling configuration.
    uint8_t                interrupt_priority; ///< Interrupt priority.
    bool                   low_power_mode;     ///< Indicates if low power mode is active.
} nrf_drv_saadc_config_t;

/**
 * @brief Driver event types.
 */
typedef enum
{
    NRF_DRV_SAADC_EVT_DONE,         ///< Event generated when the buffer is filled with samples.
    NRF_DRV_SAADC_EVT_LIMIT,        ///< Event generated after one of the limits is reached.
    NRF_DRV_SAADC_EVT_CALIBRATEDONE ///< Event generated when the calibration is complete.
} nrf_drv_saadc_evt_type_t;

/**
 * @brief Analog-to-digital converter driver done event data.
 */
typedef struct
{
    nrf_saadc_value_t * p_buffer; ///< Pointer to buffer with converted samples.
    uint16_t            size;     ///< Number of samples in the buffer.
} nrf_drv_saadc_done_evt_t;

/**
 * @brief Analog-to-digital converter driver limit event data.
 */
typedef struct
{
    uint8_t           channel;    ///< Channel on which the limit was detected.
    nrf_saadc_limit_t limit_type; ///< Type of limit detected.
} nrf_drv_saadc_limit_evt_t;

/**
 * @brief Analog-to-digital converter driver event structure.
 */
typedef struct
{
    nrf_drv_saadc_evt_type_t type; ///< Event type.
    union
    {
        nrf_drv_saadc_done_evt_t  done;  ///< Data for @ref NRF_DRV_SAADC_EVT_DONE event.
        nrf_drv_saadc_limit_evt_t limit; ///< Data for @ref NRF_DRV_SAADC_EVT_LIMIT event.
    } data;
} nrf_drv_saadc_evt_t;

/**
 * @brief ADC event handler.
 *
 * @param[in] p_event     Pointer to an ADC event. The event structure is allocated on
 *                        the stack, so it is valid only within the context of
 *                        the event handler.
 */
typedef void (* nrf_drv_saadc_event_handler_t)(nrf_drv_saadc_evt_t const * p_event);

/**
 * @brief Function for initializing the SAADC.
 *
 * @param[in] p_config      Pointer to a configuration structure. If NULL, the default one is used.
 * @param[in] event_handler Event handler provided by the user.
 *
 * @retval    NRF_SUCCESS If initialization was successful.
 * @retval    NRF_ERROR_INVALID_STATE If the driver is already initialized.
 * @retval    NRF_ERROR_INVALID_PARAM If event_handler is NULL.
 */
void nrf_drv_saadc_init(    nrf_saadc_value_t * p_buffer,
                            uint16_t size,
                            nrf_drv_saadc_event_handler_t  event_handler);


/**
 * @brief Function for getting the address of a SAMPLE SAADC task.
 *
 * @return     Task address.
 */
uint32_t nrf_drv_saadc_sample_task_get(void);

/**
 * @brief Function for initializing an SAADC channel.
 *
 * This function configures and enables the channel.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 * @retval NRF_ERROR_INVALID_STATE If the ADC was not initialized.
 * @retval NRF_ERROR_NO_MEM        If the specified channel was already allocated.
 */
void nrf_drv_saadc_channel_init(uint8_t                                  channel,
                                      nrf_saadc_channel_config_t const * const p_config);

/**
 * @brief Function for issuing conversion of data to the buffer.
 *
 * This function is non-blocking. The application is notified about filling the buffer by the event handler.
 * Conversion will be done on all enabled channels. If the ADC is in idle state, the function will set up Easy
 * DMA for the conversion. The ADC will be ready for sampling and wait for the SAMPLE task. It can be
 * triggered manually by the @ref nrf_drv_saadc_sample function or by PPI using the @ref NRF_SAADC_TASK_SAMPLE
 * task. If one buffer is already set and the conversion is ongoing, calling this function will
 * result in queuing the given buffer. The driver will start filling the issued buffer when the first one is
 * completed. If the function is called again before the first buffer is filled or calibration is in progress,
 * it will return with error.
 *
 * @param[in] buffer Result buffer.
 * @param[in] size   Buffer size in words.
 */
void nrf_drv_saadc_buffer_convert(nrf_saadc_value_t * buffer, uint16_t size);



#ifdef __cplusplus
}
#endif


/** @} */
