#ifdef MCU_NRF52


#include "nrf.h"
#include "PPI.h"
#include "nrf_drv_common.h"
#include "nrf_drv_ppi.h"
//#include "hal/gpio.h"
//#include "hal/nrf_timer.h"

//#include "nrf_drv_timer.h"

//#include "hal/Irq_common.h"
/**@brief Timer control block. */
PPI::Controller PPI::CONTROLLER;

static nrf_drv_state_t     m_drv_state;            /**< Driver state */
static uint32_t            m_channels_allocated;   /**< Bitmap representing channels availability. 1 when a channel is allocated, 0 otherwise. */
static uint8_t             m_groups_allocated;     /**< Bitmap representing groups availability. 1 when a group is allocated, 0 otherwise.*/



void PPI::Controller::nrf_drv_ppi_init()
{

    if (m_drv_state == NRF_DRV_STATE_UNINITIALIZED)
    {
        m_drv_state = NRF_DRV_STATE_INITIALIZED;
    }
}
/*
uint32_t nrf_drv_ppi_channel_enable(nrf_ppi_channel_t channel)
{
    ret_code_t err_code = NRF_SUCCESS;

    if (!is_app_channel(channel))
    {
        err_code = NRF_ERROR_INVALID_PARAM;
    }
    else if (is_programmable_app_channel(channel) && !is_allocated_channel(channel))
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    else
    {
        nrf_ppi_channel_enable(channel);
    }
    NRF_LOG_INFO("Function: %s, error code: %s.", (uint32_t)__func__, (uint32_t)NRF_LOG_ERROR_STRING_GET(err_code));
    return err_code;
}
*/

void PPI::Controller::nrf_drv_ppi_channel_assign(nrf_ppi_channel_t channel, uint32_t eep, uint32_t tep)
{
    nrf_ppi_channel_endpoint_setup(channel, eep, tep);
}

void PPI::Controller:: nrf_drv_ppi_channel_enable(nrf_ppi_channel_t channel)
{
    nrf_ppi_channel_enable(channel);
}

void PPI::Controller::nrf_drv_ppi_channel_alloc(nrf_ppi_channel_t *p_channel)
{
    uint32_t channel;
    uint32_t mask = 0;
    mask = NRF_PPI_PROG_APP_CHANNELS_MASK;
    for (channel = NRF_PPI_CHANNEL0; mask != 0; mask &= ~nrf_drv_ppi_channel_to_mask((nrf_ppi_channel_t)channel), channel++)
    {
       __disable_irq();
       if ((mask & nrf_drv_ppi_channel_to_mask((nrf_ppi_channel_t)channel)) && (((m_channels_allocated & nrf_drv_ppi_channel_to_mask((nrf_ppi_channel_t)channel)) == 0)))
       {

           m_channels_allocated |= nrf_drv_ppi_channel_to_mask((nrf_ppi_channel_t)channel);
           *p_channel = (nrf_ppi_channel_t)channel;
       }
       __enable_irq();
    }

}

#endif



