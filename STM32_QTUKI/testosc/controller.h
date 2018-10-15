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

    void setParams(uint16_t sKP, uint16_t sKI,uint16_t dKP, uint16_t dKI,uint16_t qKP, uint16_t qKI);
    void setSpeed(int32_t speed); //pulse per second
    void loop();
    void loopTest();
    void trigger(){readyToUpdate_=true;}
    void enableMotor();
    void disableMotor();
    void alignMotor();
private:
    void initPWM_();
    void initADC_();
    void initEncoder_();
    void setDuties_(uint16_t pwmA, uint16_t pwmB, uint16_t pwmC);
    uint16_t readEncoder_();

    void estimatePositionAndSpeed_();
    int32_t speedRegulator_();
    bool readyToUpdate_;
    bool motorEnabale_;

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
};

extern foc::Controller CONTROLLER;

}


#endif // FOC_H
