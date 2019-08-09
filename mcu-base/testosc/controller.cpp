#include "controller.h"
#include "command.h"

foc::Controller foc::CONTROLLER;

void foc::Controller::init()
{
    readyToUpdate_ = false;
    motorEnabale_ = false;
    dSum_ = qSum_ = sSum_ = 0;

    aFilterReg_ = 0;
    bFilterReg_ = 0;

    currentSpeed_ = 0;
    currentPosition_ = 0;
    speedRef_ = 0;
    accel_ = 0;
    windowIndex_ = 0;
    for (int i =0; i<SPEED_W_SIZE; i++) speedWindow_[i] = 0;

    /* hal */
    initPWM_();
    initADC_();
    initEncoder_();

    plot::CONTROLLER.set_d_current_error_threshold(-32000);
    plot::CONTROLLER.set_speed_error_threshold(-32000);
}

void foc::Controller::updateCurrents(int32_t ia,int32_t ib)
{
    aFilterReg_ = aFilterReg_ - (aFilterReg_ >> FILTER_SHIFT) + ia;
    ia_ = (aFilterReg_ >> FILTER_SHIFT);

    bFilterReg_ = bFilterReg_ - (bFilterReg_ >> FILTER_SHIFT) + ib;
    ib_ = (bFilterReg_ >> FILTER_SHIFT);
}

void foc::Controller::estimatePositionAndSpeed_()
{
    uint16_t p = readEncoder_();

    /* speed estimation */
    int32_t e = p - currentPosition_;
    if (e > MAX_POSITION/2) e -= MAX_POSITION;
    else if (e < -MAX_POSITION/2) e += MAX_POSITION;
    currentSpeed_ += (e - speedWindow_[windowIndex_]);
    speedWindow_[windowIndex_] = e;
    windowIndex_++;
    if (windowIndex_ >= SPEED_W_SIZE) windowIndex_ = 0;
    currentPosition_ = p;

    /* position estimation*/
    uint16_t theta = p % PULSE_2_PI;

    if (theta < PULSE_PI_2)
    {
        sinTheta_ = SINCOS[theta];
        cosTheta_ = SINCOS[PULSE_PI_2 - theta];
    }
    else if (theta < PULSE_PI)
    {
        sinTheta_ = SINCOS[PULSE_PI - theta];
        cosTheta_ = -SINCOS[theta - PULSE_PI_2];
    }
    else if (theta < PULSE_3_PI_2)
    {
        sinTheta_ = -SINCOS[theta - PULSE_PI];
        cosTheta_ = -SINCOS[PULSE_3_PI_2 - theta];
    }
    else if (theta < PULSE_2_PI)
    {
        sinTheta_ = -SINCOS[PULSE_2_PI - theta];
        cosTheta_ = SINCOS[theta -PULSE_3_PI_2];
    }
}

void foc::Controller::setSpeed(int32_t speed )
{
    setpointSpeed_ = speed;
    if (setpointSpeed_ > speedRef_) accel_ = DEFAULT_ACCEL;
    else accel_ = -DEFAULT_ACCEL;
}

int32_t foc::Controller::speedRegulator_()
{
    if (speedRef_ != setpointSpeed_)
    {
        speedRef_ += accel_;
        if ((accel_ > 0 && speedRef_ > setpointSpeed_) || (accel_ < 0 && speedRef_ < setpointSpeed_)) speedRef_ = setpointSpeed_;
    }

    int32_t e = speedRef_ - currentSpeed_;
    int32_t ret = sKP_*e + sSum_;
    if (ret > MAX_IQ_REF) ret = MAX_IQ_REF;
    else if (ret < -MAX_IQ_REF) ret = -MAX_IQ_REF;
    sSum_ += sKI_ * e;
    if (sSum_> 2*MAX_IQ_REF) sSum_ = 2*MAX_IQ_REF;
    else if (sSum_ < -2*MAX_IQ_REF) sSum_ = -2*MAX_IQ_REF;

    return ret;
}

void foc::Controller::loopTest()
{
    static uint16_t theta = 0;
    if (!readyToUpdate_) return;
    readyToUpdate_ = false;
    theta+=3;
    if (theta >= PULSE_2_PI) theta=0;

    if (theta < PULSE_PI_2)
    {
        sinTheta_ = SINCOS[theta];
        cosTheta_ = SINCOS[PULSE_PI_2 - theta];
    }
    else if (theta < PULSE_PI)
    {
        sinTheta_ = SINCOS[PULSE_PI - theta];
        cosTheta_ = -SINCOS[theta - PULSE_PI_2];
    }
    else if (theta < PULSE_3_PI_2)
    {
        sinTheta_ = -SINCOS[theta - PULSE_PI];
        cosTheta_ = -SINCOS[PULSE_3_PI_2 - theta];
    }
    else if (theta < PULSE_2_PI)
    {
        sinTheta_ = -SINCOS[PULSE_2_PI - theta];
        cosTheta_ = SINCOS[theta -PULSE_3_PI_2];
    }

    static int count =0;

    plot::CONTROLLER.plot_current_speed(sinTheta_);
    plot::CONTROLLER.plot_ref_speed(cosTheta_);
    plot::CONTROLLER.plot_q_current_error(sinTheta_);
    plot::CONTROLLER.flush();
    static int c1=0;

    if (count++>100)
    {
        c1++;
        plot::CONTROLLER.update_speed_kp(-c1);
        plot::CONTROLLER.update_speed_ki(c1);
        plot::CONTROLLER.update_speed(2*c1);
        count =0;
    }
}

void foc::Controller::loop()
{
    if (!readyToUpdate_) return;
    readyToUpdate_ = false;

    estimatePositionAndSpeed_();
    int32_t iqref = speedRegulator_();

    /* Clarke transformation */
    int32_t iA = ia_ << 4;                               //Q16.4
    int32_t iB = (_1_SQRT_3 * (ia_ + 2*ib_)) >> 12;

    /* Park transformation */
    int32_t id = iA * cosTheta_ + iB * sinTheta_;         //Q16.16
    int32_t iq = iB * cosTheta_ - iA * sinTheta_;         //Q16.16

    /* PI regulators */
    int32_t ed = (0 - id) >> 12;                        //Q16.4
    int32_t vD = (dKP_*ed + dSum_) >> 10;                 //Q16.4
    if (vD > MAX_VD) vD = MAX_VD;
    else if (vD < -MAX_VD) vD = -MAX_VD;
    dSum_ += dKI_*ed;
    if (dSum_ > 2*MAX_VD) dSum_ = 2*MAX_VD;
    else if (dSum_ < -2*MAX_VD) dSum_ = -2*MAX_VD;

    int32_t eq = ((iqref << 16) - iq) >> 12;            //Q16.4
    int32_t vQ = (qKP_*eq + qSum_) >> 10;                 //Q16.4
    if (vQ > MAX_VQ) vQ = MAX_VQ;
    else if (vQ < -MAX_VQ) vQ = -MAX_VQ;
    qSum_ += qKI_*eq;
    if (qSum_ > 2*MAX_VQ) qSum_ = 2*MAX_VQ;
    else if (qSum_ < -2*MAX_VQ) qSum_ = -2*MAX_VQ;

    /* inverse Park transformation */
    int32_t vA = (vD * cosTheta_ - vQ * sinTheta_) ;       //Q16.16
    int32_t vB = (vD * sinTheta_ + vQ * cosTheta_) ;       //Q16.16

    /* INVERSE CLARKE */
    int32_t tmp = (vA >>16) * _SQRT_3;
    int32_t VrA = vB >>12;                                 //Q16.4
    int32_t VrB = ((-vB - tmp)/2) >>12;                    //Q16.4
    int32_t VrC = ((-vB + tmp)/2) >>12;                    //Q16.4

    /* determine sector */
    uint8_t sector = 0;  //sector = aSector + 2*bSector + 4*cSector;
    if (VrA>0) sector += 1; else VrA = -VrA;
    if (VrB>0) sector += 2; else VrB = -VrB;
    if (VrC>0) sector += 4; else VrC = -VrC;//Q16.4

    uint16_t pwmA = 0, pwmB = 0, pwmC = 0;

    switch (sector)
    {
        /*TODO:*/
        case 5:
        {
            break;
        }
        case 1:
        {
            break;
        }
    }
    setDuties_(pwmA, pwmB, pwmC);
}

void foc::Controller::enableMotor()
{
    motorEnabale_ = true;
}

void foc::Controller::disableMotor()
{
    motorEnabale_ = false;
}

void foc::Controller::alignMotor()
{
    setDuties_(ALIGN_DUTY,0,0);
    /*TODO: delay 2s */
    setDuties_(0,0,0);

    currentSpeed_ = 0;
    currentPosition_ = 0;
    speedRef_ = 0;
    accel_ = 0;
    windowIndex_ = 0;
    for (int i =0; i<SPEED_W_SIZE; i++) speedWindow_[i] = 0;
}
