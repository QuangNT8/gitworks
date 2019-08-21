/*
 * Test motor step https://banlinhkien.vn/goods-8072-dong-co-buoc-1-8-do-stp-43d3014.html
 * Hardware 6N137-IR2103-IRF540-ACS712
 * PWM x 4(PA8, PA9, PB4, PB5), ADC x 2(PC0, PC1), SPI x 1, UART x 1, LED-TEST x 1
 */
#include "motor.h"
#include "Stm32_GPIO/gpio.h"

step::Motor step::MOTOR;


step::Motor::Motor()
{

}

void step::Motor::init()
{

    cmdQueue_.init(cmdBuf_,CMD_BUF_SIZE);

    GPIO::CONTROLLER.init();

    initGPIO_();
    initADC_();
    initSPI_();
    initPWM_();
    initTimer2_();
    speed = 10;
}



void step::Motor::loop()
{
    if ((command_ == NONE) && cmdQueue_.notEmpty())
    {
        command_ = cmdQueue_.pop();
        switch (command_)
        {
        case PLOT_DATA: plotData_(); break;
        case READ_ANGLE: readAS5047_(CMD_R_ANGLECOM); break;
        case RUN_PWM: runPWM_(false); break;
        case AS_MAGNITUDE: readAS5047_(CMD_R_MAG); break;;
        }
    }
    loopPC2_();
}

void step::Motor::processCommand(int command)
{
    if (cmdQueue_.available()>0) cmdQueue_.push(command);
}

void step::Motor::plotData_()
{
    command_ = NONE;
    plot::CONTROLLER.plot_ia(ia_);
    plot::CONTROLLER.plot_ib(ib_);
    plot::CONTROLLER.plot_angle(angle_);
    plot::CONTROLLER.flush();
}
