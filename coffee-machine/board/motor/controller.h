#ifndef BLDC_MOTOR_H
#define BLDC_MOTOR_H

#include "define.h"
#include "queue.h"
#include "stm32f0xx.h"
#include "generator.h"
#include "stdlib.h"

namespace motor
{

class Controller
{
public:
    enum Command{NONE=0, CALIBRATE, UPDATE_STATE, READ_ANGLE, UPDATE_CURRENT};
    enum State{UNKNOWN = 0, START_RESET, WAIT_FOR_STABILITY, FORWARD_TO_ZERO,BACKWARD_TO_ZERO, MOVE_TO_HOME, HOMING, STOP, MOVING};

    void init();
    void loop();
    void processCommand(int command);
    void setPosition(int32_t p);
    void reset();
    void startMotor();
    void stopMotor();



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


    void spiReady();
    void adcReady();

    uint16_t SPI_DATA;

private:
    uint16_t state_= UNKNOWN;
    void motorStarted_(uint32_t position);
    void motorStopped_(uint32_t position);
    void positionChanged_(uint32_t position);
    void motorReady_();
    void motorError_();

    void updateState_();
    uint8_t command_ = NONE;
    uint16_t cycleCount_ = 0;

    /* Position Control */
    void updatePosition_(int32_t p);
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

    /*zero*/
    void initGPIO_();
    bool isZero = false;

    bool zeroSensorLow_(){return ((GPIOA->IDR & GPIO_IDR_0) != 0);}
    bool zeroSensorHigh_(){return ((GPIOA->IDR & GPIO_IDR_0) == 0);}

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

