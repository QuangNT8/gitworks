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
	void plot_pref(int16_t v)
	{
		pref_= v;
		_mask |= 1;
	}
	void plot_p(int16_t v)
	{
		p_= v;
		_mask |= 2;
	}
	void plot_vref(int16_t v)
	{
		vref_= v;
		_mask |= 4;
	}
	void plot_v(int16_t v)
	{
		v_= v;
		_mask |= 8;
	}
	void update_angle(int16_t v){angle=v;angle_ready=true;}
	void update_iaofset(int16_t v){iaofset=v;iaofset_ready=true;}
	void update_ibofset(int16_t v){ibofset=v;ibofset_ready=true;}
	void loop()
	{
		bool done = true;
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;
		if (angle_ready && uart::CONTROLLER.updateField(19,angle)) angle_ready = false;
		if (iaofset_ready && uart::CONTROLLER.updateField(44,iaofset)) iaofset_ready = false;
		if (ibofset_ready && uart::CONTROLLER.updateField(43,ibofset)) ibofset_ready = false;

	}

	void flush()
	{
		if (_mask==0) return;
		dataLen_ = 0;
		_data[dataLen_++] = _mask;
		if (_mask & 1)
		{
			_data[dataLen_++] = ((pref_ >> 8) & 0xFF);
			_data[dataLen_++] = (pref_ & 0xFF);
		}
		if (_mask & 2)
		{
			_data[dataLen_++] = ((p_ >> 8) & 0xFF);
			_data[dataLen_++] = (p_ & 0xFF);
		}
		if (_mask & 4)
		{
			_data[dataLen_++] = ((vref_ >> 8) & 0xFF);
			_data[dataLen_++] = (vref_ & 0xFF);
		}
		if (_mask & 8)
		{
			_data[dataLen_++] = ((v_ >> 8) & 0xFF);
			_data[dataLen_++] = (v_ & 0xFF);
		}
		needFlush_ = true;
		_mask = 0;
	}
private:
	uint8_t _mask;
	uint8_t _data[10];
	bool done_;
	bool needFlush_;
	uint8_t dataLen_;
	int16_t angle;bool angle_ready = false;
	int16_t iaofset;bool iaofset_ready = false;
	int16_t ibofset;bool ibofset_ready = false;
	int16_t pref_;
	int16_t p_;
	int16_t vref_;
	int16_t v_;
};

extern plot::Controller CONTROLLER;

}

#endif 