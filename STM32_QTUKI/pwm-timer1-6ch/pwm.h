/*
 * encoder.h
 *
 *  Created on: Oct 10, 2018
 *      Author: vanquang
 */

#ifndef PWM_H
#define PWM_H
#include <stdint.h>

namespace pwm
{

class settup
{
public:
	void init();
    void loop();
    settup(){}
private:
    void initGpio_();

    uint32_t cntTimer1;

    uint16_t countDelayPlot_;
    uint32_t countTest_;
};
extern pwm::settup SETTUP;
}

#endif /* PWM_H */
