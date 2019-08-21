/*
 * Controller.h
 *
 *  Created on: Aug 18, 2017
 *      Author: hongsan
 */

#ifndef UART_CONTROLLER_H_
#define UART_CONTROLLER_H_
#include "queue.h"
#include <stdint.h>

#define TX_BUF_SIZE 512

namespace uart
{

class Controller
{
public:
	void init();
    void loop();
    bool sendMessage(uint16_t type, uint8_t length, const uint8_t* data);
    bool sendPlot(uint16_t type, uint8_t length, uint16_t index, uint8_t plot, const uint8_t* data);
    bool updateField(uint16_t cmd, int16_t val);
    void printMessage(char* text);
    void printfMessage(char* format, ...);
    Queue& txQueue(){return txQueue_;}

private:
    void initUART_();
    bool rxReady_();
    bool txReady_();
    uint8_t read_();
    void write_(uint8_t c);

    /*rx*/
    enum RxState{HEADER = 0, LENGTH, TYPE1, TYPE2, ID1, ID2, DATA, FOOTER};
    uint8_t rxBuffer_[256];
    uint8_t rxLength_;
    uint8_t rxState_;
    uint8_t rxIndex_;
    uint16_t rxID_;
    uint16_t rxType_;
    void processCommand_();

    /*string buffer*/
    char strBuffer_[128];
    uint8_t strLen_;
    bool needFlushStr_;

    /*tx*/
    Queue txQueue_;
    uint8_t txBuf_[TX_BUF_SIZE];
};

extern uart::Controller CONTROLLER;

}

#define UART_COMMAND_BEGIN              void uart::Controller::processCommand_(){switch (rxType_){
#define COMMAND_REG(type, command)      case type: uart_cmd_##command(rxLength_, rxBuffer_);break;
#define UART_COMMAND_END                default:break;}}
#define UART_COMMAND(command)              void uart_cmd_##command(uint8_t length, uint8_t* data)

#endif /* UART_CONTROLLER_H_ */
