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

    if (needFlushStr_ && txQueue_.available()>strLen_ + 5)
    {
        txQueue_.push(HEADER_INDICATOR);
        txQueue_.push(strLen_);
        txQueue_.push(0);
        txQueue_.push(1);

        for (int i =0;i < strLen_;i++)
        {
            txQueue_.push(strBuffer_[i]);
        }

        txQueue_.push(FOOTER_INDICATOR);
        needFlushStr_ = false;
    }
}

void uart::Controller::init()
{
    initUART_();
    txQueue_.init(txBuf_,TX_BUF_SIZE);
    rxState_ = HEADER;
    needFlushStr_ = false;
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

bool uart::Controller::sendPlot(uint16_t type, uint8_t length, uint16_t index, uint8_t plot, const uint8_t* data)
{
    if (txQueue_.available() < length + 20) return false;
    txQueue_.push(HEADER_INDICATOR);
    txQueue_.push(length+3);
    txQueue_.push((type >> 8) & 0xFF);
    txQueue_.push(type & 0xFF);
    txQueue_.push(plot);
    txQueue_.push((index >> 8) & 0xFF);
    txQueue_.push(index & 0xFF);

    for (int i =0;i < length;i++)
    {
        txQueue_.push(data[i]);
    }

    txQueue_.push(FOOTER_INDICATOR);
    return true;
}

bool uart::Controller::updateField(uint16_t cmd, int16_t val)
{
    if (txQueue_.available() < 10) return false;
    txQueue_.push(HEADER_INDICATOR);
    txQueue_.push(4);
    txQueue_.push(0);
    txQueue_.push(4);
    txQueue_.push((cmd >> 8) & 0xFF);
    txQueue_.push(cmd & 0xFF);
    txQueue_.push((val >> 8) & 0xFF);
    txQueue_.push(val & 0xFF);
    txQueue_.push(FOOTER_INDICATOR);
    return true;
}

//#include <string.h>
void uart::Controller::printMessage(char* text)
{
    if (needFlushStr_) return;
    strLen_ = 0;
    while (text[strLen_] !=0)
    {
        strBuffer_[strLen_] = text[strLen_++];
    }
    strLen_++;
    needFlushStr_ = true;
    ///int l = strlen(text) + 1;
    //this->sendMessage(1, l, (uint8_t*)text);
}

#include <stdio.h>
#include <stdarg.h>

void uart::Controller::printfMessage(char* format,...)
{
    if (needFlushStr_) return;
    va_list args;
    va_start (args, format);
    strLen_ = vsprintf(strBuffer_, format ,args) + 1;
    va_end(args);
    needFlushStr_ = true;
    //this->sendMessage(1, l, (uint8_t*)buf);
}
