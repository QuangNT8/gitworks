/*
 * Controller.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: hongsan
 */

#include "controller.h"

#define HEADER_INDICATOR	0xFE
#define FOOTER_INDICATOR	0xFD
#define MAX_PACKET_LENGTH	250
uart::Controller uart::CONTROLLER;

void uart::Controller::loop()
{
    if (txQueue_.notEmpty())
    {
        if (txReady_()) write_(txQueue_.pop());
    }

    if (rxReady_())
    {
        uint8_t c =  read_();
        switch (rxState_)
        {
        case HEADER:
            if (c == HEADER_INDICATOR) rxState_ = LENGTH;
            break;
        case LENGTH:
            if (c > MAX_PACKET_LENGTH) rxState_ = HEADER;
            else
            {
                rxLength_ = c;
                rxIndex_ = 0;
                rxState_ = TYPE1;
            }
            break;
        case TYPE1:
            rxType_ = c;
            rxState_ = TYPE2;
            break;
        case TYPE2:
            rxType_ = (rxType_<<8) + c;
            if (rxLength_>0) rxState_ = DATA;
            else rxState_ = FOOTER;
            break;
        case DATA:
            rxBuffer_[rxIndex_] = c;
            rxIndex_++;
            if (rxIndex_ == rxLength_) rxState_ = FOOTER;
            break;
        case FOOTER:
            if (c == FOOTER_INDICATOR) this->processCommand_();
            //printfMessage("Packet received type:%d, length:%d",rxType_, rxLength_);
            rxState_ = HEADER;
            break;
        default:
            rxState_ = HEADER;
            break;
        }
    }
}

void uart::Controller::init()
{
    initUART_();
    txQueue_.init(txBuf_,TX_BUF_SIZE);
    rxState_ = HEADER;
}

bool uart::Controller::sendMessage(uint16_t type, uint8_t length, const uint8_t* data)
{
    if (txQueue_.available() < length + 10) return false;
    txQueue_.push(HEADER_INDICATOR);
    txQueue_.push(length);
    txQueue_.push((type >> 8) & 0xFF);
    txQueue_.push(type & 0xFF);

    for (int i =0;i < length;i++)
    {
        txQueue_.push(data[i]);
    }

    txQueue_.push(FOOTER_INDICATOR);
    return true;
}

#include <string.h>
void uart::Controller::printMessage(char* text)
{
    int l = strlen(text) + 1;
    this->sendMessage(1, l, (uint8_t*)text);
}

#include <stdio.h>
#include <stdarg.h>

void uart::Controller::printfMessage(char* format,...)
{
    va_list args;
    va_start (args, format);
    char buf[256];
    uint16_t l = vsprintf(buf, format ,args) + 1;
    va_end(args);
    this->sendMessage(1, l, (uint8_t*)buf);
}
