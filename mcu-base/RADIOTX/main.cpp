#include "controller.h"
#include "uart/controller.h"
#include "timer/timer.h"
#include "hal/gpio.h"
#include "hal/PPI.h"
#include "SSADC/sadc.h"
#include "ACmesurements/AcMesurement.h"
#include "command.h"
#include "RADIO/radio.h"


#define oscilloscope_en true

static AcMesumentdata_t AcMesumentdata;
static uint32_t radiodata;


void update2Fields(uint32_t delay, uint16_t param1,uint16_t param2,uint16_t param3)
{
    static uint32_t count_delay = 0;
    if(count_delay>delay)
    {
        plot::CONTROLLER.update_voltage(param1);
        plot::CONTROLLER.update_currrent(param2);
        plot::CONTROLLER.update_power(param3);
        count_delay=0;
    }
    else
    {
        count_delay++;
    }
}

void update2Oscilloscope(uint32_t delay, uint16_t param1,uint16_t param2,uint16_t param3)
{
    static uint32_t count_delay = 0;
    if(count_delay>delay)
    {
        uint16_t s = plot::CONTROLLER.plot_current(param1);
        plot::CONTROLLER.plot_voltage(param2, s);
        count_delay=0;
    }
    else
    {
        count_delay++;
    }
}

int main()
{
    uint32_t powerval;
    blink::CONTROLLER.init();
    uart::CONTROLLER.init();
    plot::CONTROLLER.init();
    AcMesurement::CONTROLLER.init(&AcMesumentdata);
    radio::CONTROLLER.init(&radiodata);


    while (true)
    {
        powerval=AcMesumentdata.Ac_Voltage_data.Vac*AcMesumentdata.AC_Current_data.Current/1000;
        uart::CONTROLLER.loop();
        plot::CONTROLLER.loop();
        update2Fields(10000,AcMesumentdata.Ac_Voltage_data.Vac,AcMesumentdata.AC_Current_data.Current,powerval);
        update2Oscilloscope(10,AcMesumentdata.ac_signal_data.P030,AcMesumentdata.ac_signal_data.P031,AcMesumentdata.ac_signal_data.P029);
    }
//    return 0;
}
