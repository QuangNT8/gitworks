#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>
#include "stm32f0xx.h"
#include "define.h"
#include "core/queue.h"
#include "uart/controller.h"
#include "command.h"

namespace step
{

class Motor
{
public:
    enum Command{NONE=0, PLOT_DATA, READ_ANGLE, RUN_PWM, AS_MAGNITUDE};

    Motor();
    void init();
    void loop();
    void processCommand(int command);

    void startMotor(){motorEnabled_ = true;}
    void stopMotor(){motorEnabled_ = false;}
    void setSpeed(uint16_t temp){speed = (50000/(temp*CLIBRATE_SPEED));}
    uint16_t speed;

    void spiReady();
    void adcReady();    

    uint16_t SPI_DATA;

    /*EXTI*/
    uint32_t cntPC2;
    //uint32_t delayEXTI;

private:
    void plotData_();
    uint8_t command_ = NONE;

    void initGPIO_();    

    /* ADC */
    void initADC_();
    int16_t adcBuffer_[2];
    int16_t ia_, ib_, iA_ = 0, iB_ = 0;//iA_, iB_ dong do thuc te

    /* PWM */
    void initPWM_();
    void runPWM_(bool dir_);
    void setDuties_(uint16_t pwmAh, uint16_t pwmAl, uint16_t pwmBh, uint16_t pwmBl);
    int32_t sinTheta_, cosTheta_;
    bool motorEnabled_ = false;    

    /* SPI */
    void initSPI_();
    void readAS5047_(uint16_t command);

    /*EXTI*/
    void loopPC2_();

    void initTimer2_();

    uint16_t angle_;
    uint16_t magnitude_;

    /* command queue */
    Queue cmdQueue_;
    uint8_t cmdBuf_[CMD_BUF_SIZE];
};

extern step::Motor MOTOR;
}
#endif // MOTOR_H
