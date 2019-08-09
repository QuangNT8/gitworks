/*
 * Controller.h
 *
 *  Created on: Oct 4, 2018
 *      Author: vanquang
 */

#ifndef SPIHARDWARE_H
#define SPIHARDWARE_H
#include <stdint.h>

/*addr drv8323*/
#define ADR_FAULT_STAT      (0x00)
#define ADR_VGS_STAT        (0x01)
#define ADR_DRV_CTRL        (0x02)
#define ADR_GATE_DRV_HS     (0x03)
#define ADR_GATE_DRV_LS     (0x04)
#define ADR_OCP_CTRL        (0x05)
#define ADR_CSA_CTRL        (0x06)

namespace spi
{

class settup
{
public:
    void init();
    void loop();
    settup(){}
private:
    void initGpio_();
    void DRV8323_en_();
    void DRV8323_read_();
    void DRV8323_write_(int8_t addr,int16_t dataT);
    void DRV8323_setup_();

    void delay_(int32_t delay);

    /*DRV8323 registers*/
    uint16_t DRV8323regDrvCtrl_;
    uint16_t DRV8323regGateDrvHS_;
    uint16_t DRV8323regGateDrvLS_;
    uint16_t DRV8323regOcpCtrl_;
    uint16_t DRV8323regCsaCtrl_;

    /*data reg spi*/
    int16_t regSpi_[7];
    uint8_t countRead_;
    bool flagRxSpi_;
    bool flagWriteSpi_;

    uint16_t countDelayPlot_;
    int32_t countTest_;


};
extern spi::settup SETTUP;
}

#endif /* SPIHARDWARE_H */
