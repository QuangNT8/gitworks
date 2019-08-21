#include "controller.h"
#define HEADER1_INDICATOR	0x0D
#define HEADER2_INDICATOR	0x0A

uart2::Controller uart2::CONTROLLER;

void uart2::Controller::init()
{
    initUART_();
    rxState_ = HEADER1;
}

void uart2::Controller::loop()
{
    if (rxReady_())
    {
        uint8_t c =  read_();
        static uint8_t temp = 0;
        switch (rxState_)
        {
            case HEADER1:
                if (c == HEADER1_INDICATOR) rxState_ = HEADER2;
                break;
            case HEADER2:
                if (c == HEADER2_INDICATOR) rxState_ = DATA;
                break;
            case DATA:
                rxBuffer_[rxIndex_++] = c;
                if (rxIndex_ == 16)
                {
                    rxState_ = HEADER1;
                    rxIndex_ = 0;
                    if(temp++ >= 20)
                    {
                        scaleData_();
                        temp = 0;
                    }

                }
                break;
        }
    }
}

