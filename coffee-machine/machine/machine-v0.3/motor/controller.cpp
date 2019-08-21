#include "controller.h"
#include "command.h"

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

//    pKP_ = 200; pKI_ = 10; pKD_ = 20;//motor OK42STH47-230A no load at 17,3v
//    vKP_ = 90; vKI_ = 1;
//    KAff_ = 100; KVff_ = 100;
//    dKP_ = 3000; dKI_ = 400;
//    qKP_ = 300; qKI_ = 10;
//    V_ = 1000;
//    A_ = 5;

//    pKP_ = 1500; pKI_ = 1; pKD_ = 1;//motor OK42STH47-230A no load at 24v
//    vKP_ = 30; vKI_ = 0;
//    KAff_ = 500; KVff_ = 500;
//    dKP_ = 150; dKI_ = 20;
//    qKP_ = 100; qKI_ = 3;
//    V_ = 1000;
//    A_ = 5;

    pKP_ = 600; pKI_ = 0; pKD_ = 0;//motor OK42STH47-230A with load at 24v
    vKP_ = 30; vKI_ = 0;
    KAff_ = 100; KVff_ = 100;
    dKP_ = 800; dKI_ = 80;
    qKP_ = 70; qKI_ = 0;
    V_ = 800;
    A_ = 3;
}

void motor::Controller::loop()
{
    if ((command_ == NONE) && cmdQueue_.notEmpty())
    {
        command_ = cmdQueue_.pop();
        switch (command_)
        {
            case READ_ANGLE: readAS5047_(CMD_R_ANGLECOM);break;
            //case READ_ANGLE: readAS5047_(CMD_R_DIAAGC);break;
            //case READ_ANGLE: readAS5047_(CMD_R_MAG);break;
            case PLOT_DATA: plotData_(); break;
            case UPDATE_CURRENT: updateCurrent_();break;
            case CALIBRATE: calibrate_();break;
        }
    }
    setZero_();
}

void motor::Controller::processCommand(int command)
{
    if (cmdQueue_.available()>0) cmdQueue_.push(command);
}

void motor::Controller::plotData_()
{
    command_ = NONE;
    plot::CONTROLLER.plot_p(p_);
    plot::CONTROLLER.plot_pref(pRef_>>POSITION_SHIFT);

//    plot::CONTROLLER.plot_p(ia_);
//    plot::CONTROLLER.plot_pref(ib_);

    plot::CONTROLLER.plot_v(v_);
    plot::CONTROLLER.plot_vref(vRef_);

//    plot::CONTROLLER.plot_v(ia_);
//    plot::CONTROLLER.plot_vref(ib_);

    plot::CONTROLLER.flush();
    static uint16_t tmp =0;

    if (tmp++ > 10)
    {
        plot::CONTROLLER.update_angle(SPI_DATA & 0x3FFF);
        plot::CONTROLLER.update_iaofset(iaOffset_);
        plot::CONTROLLER.update_ibofset(ibOffset_);
        tmp =0;
    }

    currentOffset_();
}

void motor::Controller::startMotor()
{
    pSum_ = 0;
    vSum_ = 0;
    dSum_ = 0;
    qSum_ = 0;
    this->pRef_ = 0;
    this->p_ = 0;
    generator_.reset();
    motorEnabled_ = true;
}

void motor::Controller::stopMotor()
{
    motorEnabled_ = false;
}

