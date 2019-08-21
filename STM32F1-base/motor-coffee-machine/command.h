#ifndef MCUD_COMMAND_H
#define MCUD_COMMAND_H
#include "uart/controller.h"
#include"uart/oscilloscope.h"

namespace plot
{

class Controller
{
public:
    uint16_t valveisclicked;
	Controller(): _mask(0){}
void init()
	{
		needFlush_ = false;
	}
	void plot_ia(int16_t v)
	{
		ia_= v;
		_mask |= 1;
	}
	void plot_ib(int16_t v)
	{
		ib_= v;
		_mask |= 2;
	}
	void plot_angle(int16_t v)
	{
		angle_= v;
		_mask |= 4;
	}
	void update_round(int16_t v){round=v;round_ready=true;}
	void loop()
	{
		bool done = true;
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;
		if (round_ready && uart::CONTROLLER.updateField(19,round)) round_ready = false;

	}

	void flush()
	{
		if (_mask==0) return;
		dataLen_ = 0;
		_data[dataLen_++] = _mask;
		if (_mask & 1)
		{
			_data[dataLen_++] = ((ia_ >> 8) & 0xFF);
			_data[dataLen_++] = (ia_ & 0xFF);
		}
		if (_mask & 2)
		{
			_data[dataLen_++] = ((ib_ >> 8) & 0xFF);
			_data[dataLen_++] = (ib_ & 0xFF);
		}
		if (_mask & 4)
		{
			_data[dataLen_++] = ((angle_ >> 8) & 0xFF);
			_data[dataLen_++] = (angle_ & 0xFF);
		}
		needFlush_ = true;
		_mask = 0;
	}
private:
	uint8_t _mask;
	uint8_t _data[8];
	bool done_;
	bool needFlush_;
	uint8_t dataLen_;
	int16_t round;bool round_ready = false;
	int16_t ia_;
	int16_t ib_;
	int16_t angle_;
};

extern plot::Controller CONTROLLER;

}

#endif 
