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
		current_.setup(0);
		voltage_.setup(1);
		needFlush_ = false;
	}
	inline uint16_t plot_current(int16_t v){return current_.addValue(v);}
	inline void plot_current(int16_t v, uint16_t s){current_.addValue2(v,s);}
	inline void set_current_threshold(int16_t t){current_.setThreshold(t);}
	inline uint16_t plot_voltage(int16_t v){return voltage_.addValue(v);}
	inline void plot_voltage(int16_t v, uint16_t s){voltage_.addValue2(v,s);}
	inline void set_voltage_threshold(int16_t t){voltage_.setThreshold(t);}
	void plot_currentplot(int16_t v)
	{
		currentplot_= v;
		_mask |= 4;
	}
	void plot_adc4(int16_t v)
	{
		adc4_= v;
		_mask |= 8;
	}
	void update_voltage(int16_t v){voltage=v;voltage_ready=true;}
	void update_currrent(int16_t v){currrent=v;currrent_ready=true;}
	void update_power(int16_t v){power=v;power_ready=true;}
	void loop()
	{
		bool done = true;
		done &= current_.loop(done_);
		done &= voltage_.loop(done_);
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;
		if (voltage_ready && uart::CONTROLLER.updateField(3,voltage)) voltage_ready = false;
		if (currrent_ready && uart::CONTROLLER.updateField(2,currrent)) currrent_ready = false;
		if (power_ready && uart::CONTROLLER.updateField(1,power)) power_ready = false;

	}

	void flush()
	{
		if (_mask==0) return;
		dataLen_ = 0;
		_data[dataLen_++] = _mask;
		if (_mask & 4)
		{
			_data[dataLen_++] = ((currentplot_ >> 8) & 0xFF);
			_data[dataLen_++] = (currentplot_ & 0xFF);
		}
		if (_mask & 8)
		{
			_data[dataLen_++] = ((adc4_ >> 8) & 0xFF);
			_data[dataLen_++] = (adc4_ & 0xFF);
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
	int16_t voltage;bool voltage_ready = false;
	int16_t currrent;bool currrent_ready = false;
	int16_t power;bool power_ready = false;
	Oscilloscope current_;
	Oscilloscope voltage_;
	int16_t currentplot_;
	int16_t adc4_;
};

extern plot::Controller CONTROLLER;

}

#endif 