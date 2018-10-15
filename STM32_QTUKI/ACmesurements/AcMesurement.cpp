#ifdef MCU_NRF52
#include "nrf.h"
#include"AcMesurement.h"
#include "hal/PPI.h"
#include "../SSADC/sadc.h"
#include "hal/gpio.h"

#define Gainvol 7327
#define samplecurrent_ratio 1008


#define oscilloscope_en true


/**@brief ACmesurement control block. */

static nrf_ppi_channel_t     m_ppi_channel;

static AcMesumentdata_t *data_AcMesurement_ptr;
static datafilter_t *dataoutputfilted_ptr;
static AcVolDATA_t *AcVolDATA_ptr;
static AcCurDATA_t *AcCurDATA_ptr;


AcMesurement::Controller AcMesurement::CONTROLLER;

void AcMesurement::Controller::timer_irq(nrf_timer_event_t event_type, void *p_context)
{
    static uint16_t i=0;
    if(i>1000)
    {
        hal::GPIO::pinToggle(17);
        hal::GPIO::pinToggle(18);
        hal::GPIO::pinToggle(19);
        hal::GPIO::pinToggle(20);
        i=0;
    }
    else
    {
        i++;
    }
}

void AcMesurement::Controller::SSadc_irq(const nrf_drv_saadc_evt_t *p_event)
{
    uint16_t adctemp;
    static uint16_t count=0;
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        sadc::CONTROLLER.nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 4);

        data_filter(3,dataoutputfilted_ptr,(nrf_saadc_value_t *)p_event->data.done.p_buffer);
        calVol(AcVolDATA_ptr,1000,dataoutputfilted_ptr->P031);
        calI(AcCurDATA_ptr,1000,dataoutputfilted_ptr->P030);
    }
}

void AcMesurement::Controller::data_filter(uint32_t filterShift, datafilter_t *filteroutput,nrf_saadc_value_t *p_buffer)
{
    static uint32_t filter_temp0=0;
    static uint32_t filter_temp1=0;
    static uint32_t filter_temp2=0;
    static uint32_t filter_temp3=0;

    filter_temp0 = filter_temp0-(filter_temp0>>filterShift)+p_buffer[0];
    filteroutput->P028 = filter_temp0>>filterShift;

    filter_temp1 = filter_temp1-(filter_temp1>>filterShift)+p_buffer[1];
    filteroutput->P029 = filter_temp1>>filterShift;

    filter_temp2 = filter_temp2-(filter_temp2>>filterShift)+p_buffer[2];
    filteroutput->P030 = filter_temp2>>filterShift;

    filter_temp3 = filter_temp3-(filter_temp3>>filterShift)+p_buffer[3];
    filteroutput->P031 = filter_temp3>>filterShift;
}



void AcMesurement::Controller::init(AcMesumentdata_t *data_AcMesurement_ptrin)
{
    data_AcMesurement_ptr = data_AcMesurement_ptrin;
    dataoutputfilted_ptr = &data_AcMesurement_ptr->ac_signal_data;
    AcVolDATA_ptr = &data_AcMesurement_ptr->Ac_Voltage_data;
    AcCurDATA_ptr = &data_AcMesurement_ptr->AC_Current_data;

    sadc::CONTROLLER.init(SSadc_irq);
    saadc_sampling_event_init(100);
    saadc_sampling_event_enable();
}


void AcMesurement::Controller::saadc_sampling_event_init(uint32_t sample_speed_us)
{
    timer::CONTROLLER.init(timer_irq,100);
    uint32_t timer_compare_event_addr = timer::CONTROLLER.timer_compare_event_address_get();
    uint32_t saadc_sample_task_addr = sadc::CONTROLLER.sample_task_get();
    PPI::CONTROLLER.nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    PPI::CONTROLLER.nrf_drv_ppi_channel_assign(m_ppi_channel,timer_compare_event_addr,saadc_sample_task_addr);
}

void AcMesurement::Controller::saadc_sampling_event_enable()
{
    PPI::CONTROLLER.nrf_drv_ppi_channel_enable(m_ppi_channel);
}



int16_t AcMesurement::Controller::calVol(AcVolDATA_t *AcVolDATAin, int16_t numval, int16_t v)
{
    //int16_t peak,threshold;
    if (v < AcVolDATAin->minV) AcVolDATAin->minV = v;
    if (v > AcVolDATAin->maxV) AcVolDATAin->maxV = v;
    if (AcVolDATAin->indexV++ > numval)
    {
        AcVolDATAin->thresholdV = (AcVolDATAin->minV+AcVolDATAin->maxV)/2;
        AcVolDATAin->peakV = AcVolDATAin->maxV - AcVolDATAin->thresholdV;
        AcVolDATAin->minV = 32000;
        AcVolDATAin->maxV = -32000;
        AcVolDATAin->indexV = 0;
    }
    AcVolDATAin->Vin = (int16_t)((AcVolDATAin->peakV*2900)/4096)*2;
    AcVolDATAin->Vrms = AcVolDATAin->Vin*1000/Gainvol;
    AcVolDATAin->Vac = AcVolDATAin->Vrms*1000/1400;
    return true;
}

int16_t AcMesurement::Controller::calI(AcCurDATA_t *AcCurDATAin, int16_t numval, int16_t v)
{
    if (v < AcCurDATAin->minI) AcCurDATAin->minI = v;
    if (v > AcCurDATAin->maxI) AcCurDATAin->maxI = v;
    if (AcCurDATAin->indexI++ > numval)
    {
        AcCurDATAin->thresholdI = (AcCurDATAin->minI+AcCurDATAin->maxI)/2;
        AcCurDATAin->peakI = AcCurDATAin->maxI - AcCurDATAin->thresholdI;
        AcCurDATAin->minI = 32000;
        AcCurDATAin->maxI = -32000;
        AcCurDATAin->indexI = 0;
    }
    AcCurDATAin->Current = (AcCurDATAin->peakI*1000)/samplecurrent_ratio;
    return true;
}

#endif
