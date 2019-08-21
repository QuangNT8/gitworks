#include "controller.h"
#include "uart/controller.h"

motor::Controller motor::CONTROLLER;

void motor::Controller::init()
{
    cmdQueue_.init(cmdBuf_,CMD_BUF_SIZE);
    initADC_();
    initSPI_();
    initPWM_();
    initGPIO_();

    angleOffset_ = ANGLEOFFSET;/*TODO write parameter to eeprom encoder or chip*/

    for (int i =0;i< SPEED_W_SIZE; i++) vFilter_[i]=0;
    vFilterIndex_ = 0;

    pKP_ = 600; pKI_ = 0; pKD_ = 0;//motor OK42STH47-230A with load at 24v
    vKP_ = 30; vKI_ = 0;
    KAff_ = 100; KVff_ = 100;
    dKP_ = 800; dKI_ = 80;
    qKP_ = 70; qKI_ = 0;
    V_ = 800;
    A_ = 3;
    p_ = 0;
    pRef_ = 0;
}

void motor::Controller::loop()
{
    if ((command_ == NONE) && cmdQueue_.notEmpty())
    {
        command_ = cmdQueue_.pop();
        switch (command_)
        {
            case READ_ANGLE: readAS5047_(CMD_R_ANGLECOM);break;
            case UPDATE_STATE: updateState_(); break;
            case UPDATE_CURRENT: updateCurrent_();break;
            case CALIBRATE: calibrate_();break;
        }
    }

    if(motorEnabled_)
    {
        static uint16_t count = 1;
        static uint32_t iaSum = 0, ibSum = 0;
        iaSum += abs(ia_);
        ibSum += abs(ib_);
        if(count++ >= 10)
        {
            iaSum = (iaSum/10);
            ibSum = (ibSum/10);
            if ((iaSum > CURRENT_THRES) || (ibSum > CURRENT_THRES))
            {
                stopMotor();
                motorError_();
                uart::CONTROLLER.printfMessage("Over current! %d, %d", iaSum, ibSum);
            }
            count = 1;
            iaSum = 0;
            ibSum = 0;
        }
    }

    static uint16_t highCount =0, lowCount = 0;

    if (zeroSensorLow_())
    {
        highCount = 0;
        if (lowCount++ > 20)
        {
            lowCount = 0;
            isZero = true;
            if (state_ == BACKWARD_TO_ZERO)
            {
                generator_.reset();
                state_ = MOVE_TO_HOME;
            }
        }
    }
    else
    {
        lowCount = 0;
        if (highCount++ > 20)
        {
            highCount = 0;
            isZero = false;
            if (state_ == FORWARD_TO_ZERO)
            {
                generator_.reset();
                state_ = MOVE_TO_HOME;
            }
        }
    }
}

void motor::Controller::setPosition(int32_t p)
{
    if (state_ != STOP) return;
    generator_.start(A_, V_, p - pRef_);
    state_ = MOVING;
    motorStarted_(pRef_);
}

void motor::Controller::reset()
{
    motorEnabled_ = false;
    state_ = START_RESET;
}

void motor::Controller::processCommand(int command)
{
    if (cmdQueue_.available()>0) cmdQueue_.push(command);
}

void motor::Controller::updateState_()
{
    command_ = NONE;

    switch (state_)
    {
        case START_RESET:
        {
            static uint16_t count = 1;
            static uint32_t iaSum = 0, ibSum = 0;

            iaSum += adcBuffer_[0];
            ibSum += adcBuffer_[1];
            if(count++ >= 100)
            {
                iaOffset_ = (iaSum/100);
                ibOffset_ = (ibSum/100);
                count = 1;
                iaSum = 0;
                ibSum = 0;
                startMotor();
                state_ = WAIT_FOR_STABILITY;
            }
            break;
        }
        case WAIT_FOR_STABILITY:
        {
            static uint16_t count = 0;
            if (count++>100)
            {
                motorEnabled_ = true;
                count = 0;
                if(isZero)
                {
                    state_ = FORWARD_TO_ZERO;
                    generator_.start(1,200, -4096*1024);
                }
                else
                {
                    state_ = BACKWARD_TO_ZERO;
                    generator_.start(1, 200, 4096*1024);
                }
            }
            break;
        }

        case MOVE_TO_HOME:
        {
            generator_.start(1, 200, HOME_OFFSET);
            state_ = HOMING;
            break;
        }

        case HOMING:
        {
            if (generator_.state() == motor::Generator::NONE)
            {
                this->pRef_ = 0;
                this->p_ = 0;
                motorReady_();
                state_ = STOP;
            }
            break;
        }
        case MOVING:
        {
            if (generator_.state() == motor::Generator::NONE)
            {
                positionChanged_(pRef_);
                motorStopped_(pRef_);
                state_ = STOP;
            }
            else
            {
                static uint32_t oldPosition = 0;
                int32_t delta = pRef_ - oldPosition;
                if (delta > DIFF_THRES || delta < -DIFF_THRES)
                {
                    positionChanged_(pRef_);
                    oldPosition = pRef_;
                }
            }
            break;
        }
    }
}

void motor::Controller::startMotor()
{
    pSum_ = 0;
    vSum_ = 0;
    dSum_ = 0;
    qSum_ = 0;
    this->pRef_ = 0;
    this->p_ = 0;
    this->lastE_ =0;
    generator_.reset();
}

void motor::Controller::stopMotor()
{
    motorEnabled_ = false;
    state_ = UNKNOWN;
}

