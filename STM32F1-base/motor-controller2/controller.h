#ifndef FOC_CONTROLLER_H
#define FOC_CONTROLLER_H
#include <stdint.h>
#include "define.h"

namespace foc
{

class Controller
{
public:
    void init();
    void updateCurrents(int32_t ia,int32_t ib);

    void setParams(uint16_t sKP, uint16_t sKI, uint16_t dKP, uint16_t dKI, uint16_t qKP, uint16_t qKI);
    void setParams_sKP(uint16_t sKP){sKP_ = sKP;}
    void setParams_sKI(uint16_t sKI){sKI_ = sKI;}
    void setParams_dKP(uint16_t dKP){dKP_ = dKP;}
    void setParams_dKI(uint16_t dKI){dKI_ = dKI;}
    void setParams_qKP(uint16_t qKP){qKP_ = qKP;}
    void setParams_qKI(uint16_t qKI){qKI_ = qKI;}

    void setSpeed(int32_t speed); //pulse per second
    void loop();
    void loopTest();
    void trigger(){readyToUpdate_=true;}
    void startAdcconv();

    void enableMotor();
    void disableMotor();
    void alignMotor();
    void delayTimer3();//cnt++ = 1ms++
private:
    void setDuties_(uint16_t pwmA, uint16_t pwmB, uint16_t pwmC);
    uint16_t D1, D2, D3;
    int32_t dSum=0, qSum=0, sSum;
    uint8_t sector;
    int32_t iqref_;
    void foc_(uint16_t theta, int32_t ia, int32_t ib, int32_t iqref);



    void initGPIO_();
    void initPWM_();
    void initADC_();
	void initTimer3_();
    void initSPI_();
    void initEncoderTimer2_();
    //void setDuties_(uint16_t pwmA, uint16_t pwmB, uint16_t pwmC);
    uint16_t readEncoder_();
    void resetEncoder_();

	void estimatePositionAndSpeed_();
    bool readyToUpdate_;
    bool motorEnabale_;

    void setupSPI_();
    void drv8323Write_(uint8_t addr,uint16_t data);
    void drv8323Read_();
    void delay_(uint32_t delay);

    int16_t T0_,T1_,T2_;

    /* regulators: speed,d,q*/
    uint16_t dKP_, dKI_;
    uint16_t qKP_, qKI_;
    uint16_t sKP_, sKI_;
    int32_t dSum_, qSum_, sSum_;

    /* position and speed */
    uint16_t currentPosition_;
    int32_t currentSpeed_;
    int32_t setpointSpeed_;
    int32_t speedRef_;
    int8_t speedWindow_[SPEED_W_SIZE];

    uint16_t windowIndex_;
    int32_t sinTheta_, cosTheta_;
    int32_t accel_;

    /* current */
    int32_t ia_,ib_;
    int32_t aFilterReg_, bFilterReg_;

    /*adc-dma*/
    int16_t readAdc_[5];

    /*drv8323 registers*/
    uint16_t DRV8323regDrvCtrl_;
    uint16_t DRV8323regGateDrvHS_;
    uint16_t DRV8323regGateDrvLS_;
    uint16_t DRV8323regOcpCtrl_;
    uint16_t DRV8323regCsaCtrl_;

    /*data reg spi*/
    uint16_t regSpi_[7];
    uint8_t countRead_;
    bool flagRxSpi_;
    bool flagWriteSpi_;

    /*delay align motor*/
    bool flag2sAlignMotor_;
    uint32_t cnt2sAlignMotor_;

    /*Debug delay*/
    uint32_t countDelayPlot_;
    uint32_t countDelayPlot1_;
};

extern foc::Controller CONTROLLER;

}

#endif // FOC_H
