#ifndef BLDC_MOTOR_H
#define BLDC_MOTOR_H

#include "define.h"
#include "queue.h"
#include "stm32f0xx.h"
#include "generator.h"
#include "controller/controller.h"
#include "stdlib.h"
#include "command.h"

namespace motor
{

class Controller
{
public:
    enum Command{NONE=0, CALIBRATE, PLOT_DATA, READ_ANGLE, UPDATE_CURRENT};

    void init();
    void loop();
    void processCommand(int command);

    void setPosition(uint32_t p)
    {
        if (moving()) return;
        generator_.start(A_, V_, p - pMachine_);
        pMachine_ = p;
    }

    void setSpeed(uint32_t v){V_ = v;}
    void setAccel(uint32_t a){A_ = a;}
    void setPKP(uint32_t v){this->pKP_ = v;}
    void setPKI(uint32_t v){this->pKI_ = v;}
    void setPKD(uint32_t v){this->pKD_ = v;}
    void setVKP(uint32_t v){this->vKP_ = v;}
    void setVKI(uint32_t v){this->vKI_ = v;}

    void setKAff(uint32_t v){this->KAff_ = v;}
    void setKVff(uint32_t v){this->KVff_ = v;}
    void setPd(uint32_t v){this->dKP_ = v;}
    void setId(uint32_t v){this->dKI_ = v;}
    void setPq(uint32_t v){this->qKP_ = v;}
    void setIq(uint32_t v){this->qKI_ = v;}

    void startMotor();
    void stopMotor();

    void spiReady();
    void adcReady();
    bool moving(){return (generator_.state() != motor::Generator::NONE);}

    uint16_t SPI_DATA;

    /*zero*/
    enum ZERO{ZERO0=0, ZERO1, ZERO2, ZERO3};
    uint8_t zero = ZERO0;
    bool setZero = false;

private:
    void plotData_();
    uint8_t command_ = NONE;
    uint16_t cycleCount_ = 0;

    /* Position Control */
    void updatePosition_(int32_t p);
    uint32_t pMachine_;
    int32_t p_, pRef_;
    int32_t v_, vRef_, aRef_;
    int16_t vFilter_[SPEED_W_SIZE];
    uint16_t vFilterIndex_ = 0;

    uint16_t pKP_ = 0, pKI_= 0, pKD_= 0, pKV_= 0;
    uint16_t vKP_ = 0, vKI_= 0, KAff_= 0, KVff_= 0;

    uint32_t V_, A_;
    int64_t pSum_ = 0, vSum_ = 0;
    int32_t lastE_;
    uint16_t angle_;
    uint16_t angleOffset_;

    /* Current Control */    
    void updateCurrent_();
    int16_t ia_, ib_, iaFilter_ = 0, ibFilter_ = 0;
    uint16_t iaOffset_ = 2656, ibOffset_ = 2641;

    int32_t iqRef_;

    uint16_t dKP_=0, dKI_=0, qKP_=0, qKI_=0;
    int64_t dSum_ = 0, qSum_ = 0;

    void currentOffset_();
    bool iOffsetCpl_ = false;

    void overCurrent_();

    /* SPI */
    void initSPI_();
    void readAS5047_(uint16_t command);

    /* ADC */
    void initADC_();
    uint16_t adcBuffer_[3];

    /* PWM */
    void initPWM_();
    void setDuties_(uint16_t ah, uint16_t al, uint16_t bh, uint16_t bl);
    bool motorEnabled_ = false;

    /*GPIO*/
    void initGPIO_();
    void setZero_();

    void scanSensor_(int32_t position, uint8_t stage);

    /* command queue */
    Queue cmdQueue_;
    uint8_t cmdBuf_[CMD_BUF_SIZE];

    /* Generator */
    Generator generator_;

    /* calibrate */
    void calibrate_();
};

extern motor::Controller CONTROLLER;

}

#endif // BLDC_MOTOR_H

