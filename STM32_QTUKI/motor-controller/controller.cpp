#include "controller.h"
#include "command.h"

foc::Controller foc::CONTROLLER;

void foc::Controller::init()
{
    readyToUpdate_ = false;
    motorEnabale_ = false;
    dSum_ = qSum_ = sSum_ = 0;

    dKP_ = 1000;
    dKI_ = 50;
    qKP_ = 1000;
    qKI_ = 50;
    sKP_ = 1000;
    sKI_ = 50;

    aFilterReg_ = 0;
    bFilterReg_ = 0;

    currentSpeed_ = 0;
    currentPosition_ = 0;
    speedRef_ = 0;
    accel_ = 0;
    windowIndex_ = 0;
    for (int i =0; i<SPEED_W_SIZE; i++) speedWindow_[i] = 0;

    /* hal */
    initGPIO_();
    initPWM_();
    initADC_();
    initTimer3_();
    initSPI_();
    setupSPI_();
    initEncoderTimer2_();

    setSpeed(30000);
}

void foc::Controller::updateCurrents(int32_t ia,int32_t ib)
{
    aFilterReg_ = aFilterReg_ - (aFilterReg_ >> FILTER_SHIFT) + ia;
    ia_ = (aFilterReg_ >> FILTER_SHIFT);

    bFilterReg_ = bFilterReg_ - (bFilterReg_ >> FILTER_SHIFT) + ib;
    ib_ = (bFilterReg_ >> FILTER_SHIFT);
}

void foc::Controller::setParams(uint16_t sKP, uint16_t sKI,uint16_t dKP, uint16_t dKI,uint16_t qKP, uint16_t qKI)
{
    sKP_ = sKP;
    sKI_ = sKI;
    dKP_ = dKP;
    dKI_ = dKI;
    qKP_ = qKP;
    qKI_ = qKI;
}

void foc::Controller::estimatePositionAndSpeed_()
{
    uint16_t p = readEncoder_();

    /* speed estimation */
    int8_t e = p - currentPosition_;
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
    if(flag2sAlignMotor_)
    {
        alignMotor();
    }

    if(countDelayPlot_++ > 30000)
    {
        //uart::CONTROLLER.printfMessage("dKP_ = %d, dKI_ = %d, qKP_ = %d, qKI_ = %d, sKP_ = %d, sKI_ = %d", dKP_, dKI_, qKP_, qKI_, sKP_, sKI_);

        plot::CONTROLLER.update_adr00(regSpi_[0]);
        plot::CONTROLLER.update_adr01(regSpi_[1]);
        plot::CONTROLLER.update_adr02(regSpi_[2]);
        plot::CONTROLLER.update_adr03(regSpi_[3]);
        plot::CONTROLLER.update_adr04(regSpi_[4]);
        plot::CONTROLLER.update_adr05(regSpi_[5]);
        plot::CONTROLLER.update_adr06(regSpi_[6]);
        plot::CONTROLLER.update_tim2_cnt(readEncoder_());
        countDelayPlot_ = 0;
    }

    if (countDelayPlot1_++ > 100)
    {
        plot::CONTROLLER.plot_pc0_ia(speedRef_/10);
        plot::CONTROLLER.plot_pc1_ib(setpointSpeed_/10);
        plot::CONTROLLER.plot_pc2_ic(currentSpeed_/10);
        plot::CONTROLLER.plot_ia_(ia_);
        plot::CONTROLLER.plot_ib_(ib_);
        plot::CONTROLLER.flush();
        countDelayPlot1_ = 0;
    }
}

void foc::Controller::loopTestSpeed()
{
    flag2sAlignMotor_ = true;
    if(cnt2sAlignMotor_ >= 1)
    {
        cnt2sAlignMotor_ = 0;
        if (speedRef_ != setpointSpeed_)
        {
            speedRef_ += accel_;
            if ((accel_ > 0 && speedRef_ > setpointSpeed_) || (accel_ < 0 && speedRef_ < setpointSpeed_)) speedRef_ = setpointSpeed_;
        }
    }

    if (countDelayPlot1_++ > 100)
    {
        plot::CONTROLLER.plot_pc0_ia(setpointSpeed_/10);
        plot::CONTROLLER.plot_pc1_ib(speedRef_/10);
        plot::CONTROLLER.plot_pc2_ic(currentSpeed_/10);
        plot::CONTROLLER.plot_ia_(ia_);
        plot::CONTROLLER.plot_ib_(ib_);
        plot::CONTROLLER.flush();
        countDelayPlot1_ = 0;
    }

}

void foc::Controller::loop()
{
    drv8323Read_();
    startAdcconv();
    updateCurrents((2048-readAdc_[1]),(2048-readAdc_[2]));

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
    int16_t T0_,T1_,T2_;

    switch (sector)
    {
    /*TODO:*/
    case 5:
    {
        T1_ = (VrC * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T2_ = (VrA * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T0_ = _TZ - T1_ - T2_;

        pwmA = T2_ + T1_ + T0_/2;
        pwmB = T2_ + T0_/2;
        pwmC = T0_/2;

        break;
    }
    case 1:
    {
        T1_ = (VrB * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T2_ = (VrC * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T0_ = _TZ - T1_ - T2_;

        pwmA = T1_ + T0_/2;
        pwmB = T2_ + T1_ + T0_/2;
        pwmC = T0_/2;

        break;
    }
    case 3:
    {//
        T1_ = (VrA * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T2_ = (VrB * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T0_ = _TZ - T1_ - T2_;

        pwmA = T0_/2;
        pwmB = T2_ + T1_ + T0_/2;
        pwmC = T2_ + T0_/2;

        break;
    }
    case 2:
    {
        T1_ = (VrC * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T2_ = (VrA * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T0_ = _TZ - T1_ - T2_;

        pwmA = T0_/2;
        pwmB = T1_ + T0_/2;
        pwmC = T2_ + T1_ + T0_/2;

        break;
    }
    case 6:
    {
        T1_ = (VrB * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T2_ = (VrC * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T0_ = _TZ - T1_ - T2_;

        pwmA = T2_ + T0_/2;
        pwmB = T0_/2;
        pwmC = T2_ + T1_ + T0_/2;

        break;
    }
    case 4:
    {
        T1_ = (VrA * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T2_ = (VrB * _TZ_VDC_SIN_PI_3) >> M_SHIFT;
        T0_ = _TZ - T1_ - T2_;

        pwmA = T2_ + T1_ + T0_/2;
        pwmB = T0_/2;
        pwmC = T1_ + T0_/2;

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
    setDuties_(0,0,0);
}

void foc::Controller::alignMotor()
{
    flag2sAlignMotor_ = true;
    enableMotor();
    setDuties_(ALIGN_DUTY,0,0);

    if(cnt2sAlignMotor_ >= 2000)//(1khz = 0.001s)*2000 = 2s
    {
        flag2sAlignMotor_ = false;
        disableMotor();
        cnt2sAlignMotor_ = 0;
        resetEncoder_();

        currentSpeed_ = 0;
        currentPosition_ = 0;
        speedRef_ = 0;
        accel_ = 0;
        windowIndex_ = 0;
        for (int i =0; i<SPEED_W_SIZE; i++) speedWindow_[i] = 0;
    }
}
