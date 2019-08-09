#ifndef SADC_H
#define SADC_H

#include "nrf.h"
#include "nrf_drv_saadc.h"
#include "hal/nrf_saadc.h"
#include "nrf_drv_saadc.h"

namespace sadc
{
    class Controller
    {
    public:
        void init(nrf_drv_saadc_event_handler_t saadc_callback);
        void nrf_drv_saadc_init(    nrf_saadc_value_t * p_buffer,
                                    uint16_t size,
                                    nrf_drv_saadc_event_handler_t  event_handler);

        uint32_t sample_task_get(void);
        void nrf_drv_saadc_buffer_convert(nrf_saadc_value_t * p_buffer, uint16_t size);
    private:
        void nrf_drv_saadc_channel_init(uint8_t                                  channel,
                                        nrf_saadc_channel_config_t const * const p_config);
        void saadc_sampling_event_init(void);
        void nrf_saadc_channel_init(uint8_t channel, nrf_saadc_channel_config_t const * const config);
    };

extern sadc::Controller CONTROLLER;

}
#endif // CONTROLLER_H
