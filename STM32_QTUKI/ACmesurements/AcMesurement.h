#ifndef AcMesurement_H
#define AcMesurement_H
#include "nrf.h"
#include "../timer/timer.h"
#include "hal/PPI.h"
#include "../SSADC/sadc.h"
typedef struct
{
    uint16_t indexV;
    int16_t maxV;
    int16_t minV;
    int16_t peakV;
    int16_t thresholdV;
    int16_t voltage;
    int16_t Vin;
    int16_t Vrms;
    int16_t Vac;
} AcVolDATA_t;

typedef struct
{
    int16_t Current;
    uint16_t indexI;
    int16_t maxI;
    int16_t minI;
    int16_t peakI;
    int16_t thresholdI;
} AcCurDATA_t;

typedef struct
{
    int16_t P028;
    int16_t P029;
    int16_t P030;
    int16_t P031;
} datafilter_t;

typedef struct
{
    datafilter_t ac_signal_data;
    AcCurDATA_t AC_Current_data;
    AcVolDATA_t Ac_Voltage_data;
} AcMesumentdata_t;

namespace AcMesurement
{

    class Controller
    {
    public:

        static int16_t calVol(AcVolDATA_t *AcVolDATAin, int16_t numval, int16_t v);
        static int16_t calI(AcCurDATA_t *AcCurDATAin, int16_t numval, int16_t v);
        void init(AcMesumentdata_t *data_AcMesurement_ptrin);
        void loop();

    private:
        static void data_filter(uint32_t filterShift,datafilter_t *filteroutput,nrf_saadc_value_t *p_buffer);
        void saadc_sampling_event_init(uint32_t sample_speed_us);
        void saadc_sampling_event_enable(void);
        static void timer_irq(nrf_timer_event_t event_type, void* p_context);
        static void SSadc_irq(nrf_drv_saadc_evt_t const * p_event);
    };

extern AcMesurement::Controller CONTROLLER;

}
#endif // CONTROLLER_H
