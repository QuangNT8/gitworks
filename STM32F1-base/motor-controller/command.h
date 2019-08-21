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
	void plot_pc0_ia(int16_t v)
	{
		pc0_ia_= v;
		_mask |= 1;
	}
	void plot_pc1_ib(int16_t v)
	{
		pc1_ib_= v;
		_mask |= 2;
	}
	void plot_pc2_ic(int16_t v)
	{
		pc2_ic_= v;
		_mask |= 4;
	}
	void plot_ia_(int16_t v)
	{
		ia__= v;
		_mask |= 8;
	}
	void plot_ib_(int16_t v)
	{
		ib__= v;
		_mask |= 16;
	}
	void update_tim2_cnt(int16_t v){tim2_cnt=v;tim2_cnt_ready=true;}
	void update_adr00(int16_t v){adr00=v;adr00_ready=true;}
	void update_adr01(int16_t v){adr01=v;adr01_ready=true;}
	void update_adr02(int16_t v){adr02=v;adr02_ready=true;}
	void update_adr03(int16_t v){adr03=v;adr03_ready=true;}
	void update_adr04(int16_t v){adr04=v;adr04_ready=true;}
	void update_adr05(int16_t v){adr05=v;adr05_ready=true;}
	void update_adr06(int16_t v){adr06=v;adr06_ready=true;}
	void loop()
	{
		bool done = true;
		done_ = done;
		if (needFlush_ && uart::CONTROLLER.sendMessage(2, dataLen_,_data)) needFlush_ = false;
		if (tim2_cnt_ready && uart::CONTROLLER.updateField(27,tim2_cnt)) tim2_cnt_ready = false;
		if (adr00_ready && uart::CONTROLLER.updateField(26,adr00)) adr00_ready = false;
		if (adr01_ready && uart::CONTROLLER.updateField(25,adr01)) adr01_ready = false;
		if (adr02_ready && uart::CONTROLLER.updateField(24,adr02)) adr02_ready = false;
		if (adr03_ready && uart::CONTROLLER.updateField(23,adr03)) adr03_ready = false;
		if (adr04_ready && uart::CONTROLLER.updateField(22,adr04)) adr04_ready = false;
		if (adr05_ready && uart::CONTROLLER.updateField(21,adr05)) adr05_ready = false;
		if (adr06_ready && uart::CONTROLLER.updateField(20,adr06)) adr06_ready = false;

	}

	void flush()
	{
		if (_mask==0) return;
		dataLen_ = 0;
		_data[dataLen_++] = _mask;
		if (_mask & 1)
		{
			_data[dataLen_++] = ((pc0_ia_ >> 8) & 0xFF);
			_data[dataLen_++] = (pc0_ia_ & 0xFF);
		}
		if (_mask & 2)
		{
			_data[dataLen_++] = ((pc1_ib_ >> 8) & 0xFF);
			_data[dataLen_++] = (pc1_ib_ & 0xFF);
		}
		if (_mask & 4)
		{
			_data[dataLen_++] = ((pc2_ic_ >> 8) & 0xFF);
			_data[dataLen_++] = (pc2_ic_ & 0xFF);
		}
		if (_mask & 8)
		{
			_data[dataLen_++] = ((ia__ >> 8) & 0xFF);
			_data[dataLen_++] = (ia__ & 0xFF);
		}
		if (_mask & 16)
		{
			_data[dataLen_++] = ((ib__ >> 8) & 0xFF);
			_data[dataLen_++] = (ib__ & 0xFF);
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
	int16_t tim2_cnt;bool tim2_cnt_ready = false;
	int16_t adr00;bool adr00_ready = false;
	int16_t adr01;bool adr01_ready = false;
	int16_t adr02;bool adr02_ready = false;
	int16_t adr03;bool adr03_ready = false;
	int16_t adr04;bool adr04_ready = false;
	int16_t adr05;bool adr05_ready = false;
	int16_t adr06;bool adr06_ready = false;
	int16_t pc0_ia_;
	int16_t pc1_ib_;
	int16_t pc2_ic_;
	int16_t ia__;
	int16_t ib__;
};

extern plot::Controller CONTROLLER;

}

#endif 