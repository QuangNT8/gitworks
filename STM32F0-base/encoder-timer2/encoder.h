/*
 * encoder.h
 *
 *  Created on: Oct 9, 2018
 *      Author: vanquang
 */

#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>

namespace encoder
{

class settup
{
public:
	void init();
    void loop();
    settup(){}
private:
    void initGpio_();
    uint16_t cntEncoder_;

    uint16_t countDelayPlot_;
    int32_t countTest_;
};
extern encoder::settup SETTUP;
}

#endif /* ENCODER_H */
