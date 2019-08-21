#ifndef MCUD_COMMAND_H
#define MCUD_COMMAND_H
#include "../uart/controller.h"
#include"../uart/oscilloscope.h"

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
	void update_field_1(int16_t v){field_1=v;field_1_ready=true;}
	void update_field_2(int16_t v){field_2=v;field_2_ready=true;}
	void update_field_3(int16_t v){field_3=v;field_3_ready=true;}
	void loop()
	{
		bool done = true;
		done &= current_.loop(done_);
		done &= voltage_.loop(done_);
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;
		if (field_1_ready && uart::CONTROLLER.updateField(3,field_1)) field_1_ready = false;
		if (field_2_ready && uart::CONTROLLER.updateField(2,field_2)) field_2_ready = false;
		if (field_3_ready && uart::CONTROLLER.updateField(1,field_3)) field_3_ready = false;

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
	int16_t field_1;bool field_1_ready = false;
	int16_t field_2;bool field_2_ready = false;
	int16_t field_3;bool field_3_ready = false;
	Oscilloscope current_;
	Oscilloscope voltage_;
	int16_t currentplot_;
	int16_t adc4_;
};

extern plot::Controller CONTROLLER;

}

#endif 
