/*QuangNT*/
/**
 * @addtogroup nrf_saadc SAADC HAL and driver
 * @ingroup    nrf_drivers
 * @brief      @tagAPI52 Successive Approximation Analog-to-Digital Converter (SAADC) APIs.
 * @details The SAADC HAL provides basic APIs for accessing the registers of the SAADC peripheral.
 * The SAADC driver provides APIs on a higher level.
 *
 * @defgroup nrf_drv_saadc SAADC driver
 * @{
 * @ingroup  nrf_saadc
 *
 * @brief    @tagAPI52 Successive Approximation Analog-to-Digital Converter (SAADC) driver.
 */

#ifndef NRF_DRV_SAADC_H__
#define NRF_DRV_SAADC_H__


#include "../hal/nrf_saadc.h"
#include "../hal/nrf_drv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Value that should be set as high limit to disable limit detection.
 */
#define NRF_DRV_SAADC_LIMITH_DISABLED (2047)
/**
 * @brief Value that should be set as low limit to disable limit detection.
 */
#define NRF_DRV_SAADC_LIMITL_DISABLED (-2048)

/**
 * @brief Macro for setting @ref nrf_drv_saadc_config_t to default settings.
 */
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
#define NRF_DRV_SAADC_DEFAULT_CONFIG                                        \
    {                                                                       \
    .resolution         = (nrf_saadc_resolution_t)1,  \
    .oversample         = (nrf_saadc_oversample_t)0,  \
    .interrupt_priority = 7,                        \
    .low_power_mode     = 0                              \
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
 * @brief Function for uninitializing the SAADC.
 *
 * This function stops all ongoing conversions and disables all channels.
 */
void nrf_drv_saadc_uninit(void);


/**
 * @brief Function for getting the address of a SAMPLE SAADC task.
 *
 * @return     Task address.
 */
uint32_t nrf_drv_saadc_sample_task_get(void);

/**
 * @brief Function for retrieving the SAADC state.
 *
 * @retval true  If the ADC is busy.
 * @retval false If the ADC is ready.
 */
bool nrf_drv_saadc_is_busy(void);

/**
 * @brief Function for aborting ongoing and buffered conversions.
 * @note @ref NRF_DRV_SAADC_EVT_DONE event will be generated if there is a conversion in progress.
 *       Event will contain number of words in the sample buffer.
 */
void nrf_drv_saadc_abort(void);

/**
 * @brief Function for setting the SAADC channel limits.
 *        When limits are enabled and the result exceeds the defined bounds, the limit handler function is called.
 *
 * @param[in] channel SAADC channel number.
 * @param[in] limit_low Lower limit (valid values from @ref NRF_DRV_SAADC_LIMITL_DISABLED to
 *            @ref NRF_DRV_SAADC_LIMITH_DISABLED). Conversion results below this value will trigger
 *            the handler function. Set to @ref NRF_DRV_SAADC_LIMITL_DISABLED to disable this limit.
 * @param[in] limit_high Upper limit (valid values from @ref NRF_DRV_SAADC_LIMITL_DISABLED to
 *            @ref NRF_DRV_SAADC_LIMITH_DISABLED). Conversion results above this value will trigger
 *            the handler function. Set to @ref NRF_DRV_SAADC_LIMITH_DISABLED to disable this limit.
 */
void nrf_drv_saadc_limits_set(uint8_t channel, int16_t limit_low, int16_t limit_high);

#ifdef __cplusplus
}
#endif

#endif // NRF_DRV_SAADC_H__

/** @} */
