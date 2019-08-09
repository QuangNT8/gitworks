/*
 * Controller.h
 *
 *  Created on: Oct 4, 2018
 *      Author: vanquang
 */

#ifndef SPI_CONTROLLER_H_
#define SPI_CONTROLLER_H_
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

class Controller
{
public:
    void init();
    void loop();
private:
    void initSpi_();
    int16_t DRV8323_read(int8_t regAdr);
    void DRV8323_write(int8_t addr,int16_t dataT);
    void DRV8323_setup();

    /*default values for the DRV8323 registers*/
    uint16_t DRV8323regDrvCtrl =
      0 << 9  | //DIS_CPUV
      1 << 8  | //DIS_GDF
      1 << 7  | //OTW_REP
      0 << 5  | //PWM_MODE
      0 << 4  | //1PWM_COM
      0 << 3  | //1PWM_DIR
      0 << 2  | //COAST
      0 << 1  | //BRAKE
      0;        //CLR_FLT

    uint16_t DRV8323regGateDrvHS =
      3 << 8  | //LOCK
      11 << 4 | //IDRIVEP_HS
      15;       //IDRIVEN_HS

    uint16_t DRV8323regGateDrvLS =
      1 << 10 | //CBC
      2 << 8  | //TDRIVE
      15 << 4 | //IDRIVEP_LS
      15;       //IDRIVEN_LS

    uint16_t DRV8323regOcpCtrl =
      0 << 10 | //TRETRY
      3 << 8  | //DEAD_TIME           	//400ns
      1 << 6  | //OCP_MODE
      2 << 4  | //OCP_DEG
      0;        //VDS_LVL

    uint16_t DRV8323regCsaCtrl =		//0b 0010 0000 0011
      0 << 10 | //CSA_FET
      1 << 9  | //VREF_DIV
      0 << 8  | //LS_REF
      0 << 6  | //CSA_GAIN				//bit 6,7 = 11 Gcsa = 40	bit 6,7 = 0 Gcsa = 5
      0 << 5  | //DIS_SEN
      0 << 4  | //CSA_CAL_A
      0 << 3  | //CCSA_CAL_B
      0 << 2  | //CCSA_CAL_C
      3;        //CSEN_LVL
};
extern spi::Controller CONTROLLER;
}

#endif /* SPI_CONTROLLER_H_ */
