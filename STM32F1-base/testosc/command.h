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
		speed_error_.setup(2);
		d_current_error_.setup(3);
		needFlush_ = false;
	}
	void plot_current_speed(int16_t v)
	{
		current_speed_= v;
		_mask |= 1;
	}
	void plot_ref_speed(int16_t v)
	{
		ref_speed_= v;
		_mask |= 2;
	}
	inline uint16_t plot_speed_error(int16_t v){return speed_error_.addValue(v);}
	inline void plot_speed_error(int16_t v, uint16_t s){speed_error_.addValue2(v,s);}
	inline void set_speed_error_threshold(int16_t t){speed_error_.setThreshold(t);}
	inline uint16_t plot_d_current_error(int16_t v){return d_current_error_.addValue(v);}
	inline void plot_d_current_error(int16_t v, uint16_t s){d_current_error_.addValue2(v,s);}
	inline void set_d_current_error_threshold(int16_t t){d_current_error_.setThreshold(t);}
	void plot_q_current_error(int16_t v)
	{
		q_current_error_= v;
		_mask |= 16;
	}
	void update_speed_kp(int16_t v){speed_kp=v;speed_kp_ready=true;}
	void update_speed_ki(int16_t v){speed_ki=v;speed_ki_ready=true;}
	void update_speed(int16_t v){speed=v;speed_ready=true;}
	void loop()
	{
		bool done = true;
		done &= speed_error_.loop(done_);
		done &= d_current_error_.loop(done_);
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;
		if (speed_kp_ready && uart::CONTROLLER.updateField(3,speed_kp)) speed_kp_ready = false;
		if (speed_ki_ready && uart::CONTROLLER.updateField(2,speed_ki)) speed_ki_ready = false;
		if (speed_ready && uart::CONTROLLER.updateField(1,speed)) speed_ready = false;

	}

	void flush()
	{
		if (_mask==0) return;
		dataLen_ = 0;
		_data[dataLen_++] = _mask;
		if (_mask & 1)
		{
			_data[dataLen_++] = ((current_speed_ >> 8) & 0xFF);
			_data[dataLen_++] = (current_speed_ & 0xFF);
		}
		if (_mask & 2)
		{
			_data[dataLen_++] = ((ref_speed_ >> 8) & 0xFF);
			_data[dataLen_++] = (ref_speed_ & 0xFF);
		}
		if (_mask & 16)
		{
			_data[dataLen_++] = ((q_current_error_ >> 8) & 0xFF);
			_data[dataLen_++] = (q_current_error_ & 0xFF);
		}
		needFlush_ = true;
		_mask = 0;
	}
private:
	uint8_t _mask;
	uint8_t _data[12];
	bool done_;
	bool needFlush_;
	uint8_t dataLen_;
	int16_t speed_kp;bool speed_kp_ready = false;
	int16_t speed_ki;bool speed_ki_ready = false;
	int16_t speed;bool speed_ready = false;
	int16_t current_speed_;
	int16_t ref_speed_;
	Oscilloscope speed_error_;
	Oscilloscope d_current_error_;
	int16_t q_current_error_;
};

extern plot::Controller CONTROLLER;

}

#endif 