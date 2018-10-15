#include "controller.h"
#include "command.h"

foc::Controller foc::CONTROLLER;

void foc::Controller::init()
{
    readyToUpdate_ = false;
    motorEnabale_ = false;
    dSum_ = qSum_ = sSum_ = 0;

    iqref_ = 3000;
    dKP_ = 1000;
    dKI_ = 50;
    qKP_ = 1000;
    qKI_ = 50;

    aFilterReg_ = 0;
    bFilterReg_ = 0;

    currentSpeed_ = 0;
    currentPosition_ = 0;
    speedRef_ = 0;
    accel_ = 0;
    windowIndex_ = 0;

    /* hal */
    initGPIO_();
    initPWM_();
    initADC_();
    initTimer3_();
    initSPI_();
    setupSPI_();
    initEncoderTimer2_();
}

void foc::Controller::updateCurrents(int32_t ia,int32_t ib)
{
    aFilterReg_ = aFilterReg_ - (aFilterReg_ >> 5) + ia;
    ia_ = (aFilterReg_ >> 5);

    bFilterReg_ = bFilterReg_ - (bFilterReg_ >> 5) + ib;
    ib_ = (bFilterReg_ >> 5);
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
}

void foc::Controller::foc_(uint16_t theta, int32_t ia, int32_t ib, int32_t iqref)
{
    int32_t sinTheta,cosTheta;

    /* calcule sin(theta) and cos(theta) */
    if (theta > 4*M_PULSE_PI_2) theta %= (4*M_PULSE_PI_2);
    if (theta < M_PULSE_PI_2)
    {
        sinTheta = sineTable[theta];
        cosTheta = sineTable[M_PULSE_PI_2 - theta];
    }
    else if (theta < 2*M_PULSE_PI_2)
    {
        sinTheta = sineTable[2*M_PULSE_PI_2 - theta];
        cosTheta = -sineTable[theta - M_PULSE_PI_2];
    }
    else if (theta < 3*M_PULSE_PI_2)
    {
        sinTheta = -sineTable[theta - 2*M_PULSE_PI_2];
        cosTheta = -sineTable[3*M_PULSE_PI_2 - theta];
    }
    else if (theta < 4*M_PULSE_PI_2)
    {
        sinTheta = -sineTable[4*M_PULSE_PI_2 - theta];
        cosTheta = sineTable[theta -3*M_PULSE_PI_2];
    }

    /* Clarke transformation */
    int32_t iA = ia << 4;                               		//Q16.4
    int32_t iB = (M_1_SQRT3 * (ia + 2*ib)) >> 12;       		//Q16.4

    /* Park transformation */
    int32_t id = iA * cosTheta + iB * sinTheta;               	//Q16.16
    int32_t iq = iB * cosTheta - iA * sinTheta;               	//Q16.16

    /* PI regulators */
    int32_t ed = (0 - id) >> 12;                        		//Q16.4
    int32_t vD = (dKP_*ed + dSum) >> 10;                		//Q16.4
    if (vD > VD_MAX) vD = VD_MAX;
    else if (vD < -VD_MAX) vD = -VD_MAX;
    dSum += dKI_*ed;
    if (dSum > VD_MAX) dSum = VD_MAX;
    else if (dSum < -VD_MAX) dSum = -VD_MAX;

    int32_t eq = ((iqref << 16) - iq) >> 12;            		//Q16.4
    int32_t vQ = (qKP_*eq + qSum) >> 10;                 		//Q16.4
    if (vQ > VQ_MAX) vQ = VQ_MAX;
    else if (vQ < -VQ_MAX) vQ = -VQ_MAX;
    qSum += qKI_*eq;
    if (qSum > VQ_MAX) qSum = VQ_MAX;
    else if (qSum < -VQ_MAX) qSum = -VQ_MAX;

    /* inverse Park transformation */
    int32_t vA = (vD * cosTheta - vQ * sinTheta) ;       		//Q16.16
    int32_t vB = (vD * sinTheta + vQ * cosTheta) ;       		//Q16.16

    /* INVERSE CLARKE */
    int32_t tmp = (vA >>16) * M_SQRT3_Q;						//Q16.16
    int32_t Vra = vB >>12;                                  	//Q16.4
    int32_t Vrc = ((-vB + tmp)/2) >>12;                    		//Q16.4
    int32_t Vrb = ((-vB - tmp)/2) >>12;                    		//Q16.4

    /* sector indefication */
    sector = 0;  //sector = aSector + 2*bSector + 4*cSector;
    if (Vra>0) sector += 1; else Vra = -Vra;
    if (Vrb>0) sector += 2; else Vrb = -Vrb;
    if (Vrc>0) sector += 4; else Vrc = -Vrc;

    int16_t T0,T1,T2;

    switch (sector)
        {
            case 5:
            {
                T1 = (Vrc * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T2 = (Vra * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T0 = M_TZ - T1 - T2;

                D1 = T2 + T1 + T0/2;
                D2 = T2 + T0/2;
                D3 = T0/2;

                break;
            }
            case 1:
            {
                T1 = (Vrb * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T2 = (Vrc * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T0 = M_TZ - T1 - T2;

                D1 = T1 + T0/2;
                D2 = T2 + T1 + T0/2;
                D3 = T0/2;

                break;
            }
            case 3:
            {//
                T1 = (Vra * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T2 = (Vrb * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T0 = M_TZ - T1 - T2;

                D1 = T0/2;
                D2 = T2 + T1 + T0/2;
                D3 = T2 + T0/2;

                break;
            }
            case 2:
            {
                T1 = (Vrc * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T2 = (Vra * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T0 = M_TZ - T1 - T2;

                D1 = T0/2;
                D2 = T1 + T0/2;
                D3 = T2 + T1 + T0/2;

                break;
            }
            case 6:
            {
                T1 = (Vrb * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T2 = (Vrc * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T0 = M_TZ - T1 - T2;

                D1 = T2 + T0/2;
                D2 = T0/2;
                D3 = T2 + T1 + T0/2;

                break;
            }
            case 4:
            {
                T1 = (Vra * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T2 = (Vrb * M_TZ_VDC_SIN_PI_3) >> M_SHIFT;
                T0 = M_TZ - T1 - T2;

                D1 = T2 + T1 + T0/2;
                D2 = T0/2;
                D3 = T1 + T0/2;

                break;
            }

        }
    if (D1>M_TZ) D1 = M_TZ;
    if (D2>M_TZ) D2 = M_TZ;
    if (D3>M_TZ) D3 = M_TZ;

    foc::Controller::setDuties_(D1,D2,D3);
}

void foc::Controller::setSpeed(int32_t speed)
{
    iqref_ = speed;
}

void foc::Controller::loop()
{
    if ((!readyToUpdate_) || (!motorEnabale_)) return;
    readyToUpdate_ = false;
    estimatePositionAndSpeed_();
    foc_(readEncoder_(), ia_, ib_, iqref_);
}

void foc::Controller::loopTest()
{
    drv8323Read_();
    startAdcconv();
    updateCurrents((2048-readAdc_[1]),(2048-readAdc_[2]));

    if(flag2sAlignMotor_)
    {
        alignMotor();
    }

    if(countDelayPlot_++ > 30000)
    {
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
        plot::CONTROLLER.plot_pc0_ia(2048-readAdc_[1]);
        plot::CONTROLLER.plot_pc1_ib(2048-readAdc_[2]);
        plot::CONTROLLER.plot_pc2_ic(currentSpeed_/10);
        plot::CONTROLLER.plot_ia_(ia_);
        plot::CONTROLLER.plot_ib_(ib_);
        plot::CONTROLLER.flush();
        countDelayPlot1_ = 0;
    }
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
    motorEnabale_ = true;
    setDuties_(800,0,0);

    if(cnt2sAlignMotor_ >= 2000)//(1khz = 0.001s)*2000 = 2s
    {
        flag2sAlignMotor_ = false;
        motorEnabale_ = false;
        cnt2sAlignMotor_ = 0;
        resetEncoder_();
        setDuties_(0,0,0);

        currentSpeed_ = 0;
        currentPosition_ = 0;
        speedRef_ = 0;
        accel_ = 0;
        windowIndex_ = 0;
    }
}
