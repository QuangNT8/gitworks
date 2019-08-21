#ifndef PPI_H
#define PPI_H

#include "nrf_ppi.h"


namespace PPI
{
    class Controller
    {
    public:
        void init();
        void loop();
        void nrf_drv_ppi_init(void);
        void nrf_drv_ppi_channel_assign(nrf_ppi_channel_t channel, uint32_t eep, uint32_t tep);
        void nrf_drv_ppi_channel_enable(nrf_ppi_channel_t channel);
        void nrf_drv_ppi_channel_alloc(nrf_ppi_channel_t * p_channel);
    private:
        void pri_init();

    };

extern PPI::Controller CONTROLLER;

}
#endif // CONTROLLER_H
