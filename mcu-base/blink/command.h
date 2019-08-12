#ifndef MCUD_COMMAND_H
#define MCUD_COMMAND_H
#include "uart/controller.h"
#include"uart/oscilloscope.h"

namespace plot
{

class Controller
{
public:
	Controller(): _mask(0){}
void init()
	{
		needFlush_ = false;
	}
	void plot_plot_1(int16_t v)
	{
		plot_1_= v;
		_mask |= 1;
	}
	void plot_plot_2(int16_t v)
	{
		plot_2_= v;
		_mask |= 2;
	}
	void loop()
	{
		bool done = true;
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;

	}

	void flush()
	{
		if (_mask==0) return;
		dataLen_ = 0;
		_data[dataLen_++] = _mask;
		if (_mask & 1)
		{
			_data[dataLen_++] = ((plot_1_ >> 8) & 0xFF);
			_data[dataLen_++] = (plot_1_ & 0xFF);
		}
		if (_mask & 2)
		{
			_data[dataLen_++] = ((plot_2_ >> 8) & 0xFF);
			_data[dataLen_++] = (plot_2_ & 0xFF);
		}
		needFlush_ = true;
		_mask = 0;
	}
private:
	uint8_t _mask;
	uint8_t _data[6];
	bool done_;
	bool needFlush_;
	uint8_t dataLen_;
	int16_t plot_1_;
	int16_t plot_2_;
};

extern plot::Controller CONTROLLER;

}

#endif 