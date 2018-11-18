/**
 * Copyright (c) 2014 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/** @file
 * @defgroup nrf_adc_example main.c
 * @{
 * @ingroup nrf_adc_example
 * @brief ADC Example Application main file.
 *
 * This file contains the source code for a sample application using ADC.
 *
 * @image html example_board_setup_a.jpg "Use board setup A for this example."
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf.h"
//#include "nrf_drv_saadc.h"
#include "../adc/nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "boards.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_power.h"
#include "app_pwm.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define SAADC_SAMPLE_RATE		        250     /**< SAADC sample rate in ms. */

#define SAADC_SAMPLES_IN_BUFFER        4
volatile uint8_t state = 1;

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(0);
static nrf_saadc_value_t     m_buffer_pool[2][SAADC_SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t     m_ppi_channel;
static uint32_t              m_adc_evt_counter;

APP_PWM_INSTANCE(PWM1,1);                   // Create the instance "PWM1" using TIMER1.

static volatile bool ready_flag;            // A flag indicating PWM status.

void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}

void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

}
void nrf_drv_common_irq_enable(IRQn_Type IRQn, uint8_t priority)
{


}

uint32_t timer_init(nrf_drv_timer_t const * const p_instance,
                              nrf_drv_timer_config_t const * p_config,
                              nrf_timer_event_handler_t timer_event_handler)
{

    ret_code_t err_code;

//    p_cb->handler = timer_event_handler;

    NVIC_SetPriority(TIMER0_IRQn, 7);
    NVIC_ClearPendingIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER0_IRQn);
//    nrf_drv_common_irq_enable(nrf_drv_get_IRQn(p_instance->p_reg),
//        p_config->interrupt_priority);
    NRF_TIMER0->MODE = TIMER_DEFAULT_CONFIG_MODE;
    NRF_TIMER0->BITMODE = TIMER_DEFAULT_CONFIG_BIT_WIDTH;
    NRF_TIMER0->PRESCALER = TIMER_DEFAULT_CONFIG_FREQUENCY;
//    nrf_timer_mode_set(p_instance->p_reg, p_config->mode);
//    nrf_timer_bit_width_set(p_instance->p_reg, p_config->bit_width);
//    nrf_timer_frequency_set(p_instance->p_reg, p_config->frequency);
}

void saadc_sampling_event_init(void)
{
    ret_code_t err_code;
    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_config = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_config.frequency = NRF_TIMER_FREQ_31250Hz;
    err_code = nrf_drv_timer_init(&m_timer, &timer_config, timer_handler);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer,SAADC_SAMPLE_RATE);
    nrf_drv_timer_extended_compare(&m_timer, NRF_TIMER_CC_CHANNEL0, ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer, NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_event_addr = nrf_drv_saadc_sample_task_get();

//    NRF_LOG_INFO("timer_compare_event_addr: %d\r\n",timer_compare_event_addr);
//    NRF_LOG_INFO("saadc_sample_event_addr: %d\r\n",saadc_sample_event_addr);

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    nrf_ppi_channel_endpoint_setup(m_ppi_channel, timer_compare_event_addr, saadc_sample_event_addr);
}



void saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}


void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;
        uint16_t adc_value;
        uint8_t value[SAADC_SAMPLES_IN_BUFFER*2];
        uint8_t bytes_to_send;

        // set buffers
        nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAADC_SAMPLES_IN_BUFFER);

        // print samples on hardware UART and parse data for BLE transmission
        NRF_LOG_INFO("ADC event number: %d\r\n",(int)m_adc_evt_counter);
        for (int i = 0; i < SAADC_SAMPLES_IN_BUFFER; i++)
        {
            //printf("%d\r\n", p_event->data.done.p_buffer[i]);
            NRF_LOG_INFO("%d", p_event->data.done.p_buffer[i]);

            adc_value = p_event->data.done.p_buffer[i];
            value[i*2] = adc_value;
            value[(i*2)+1] = adc_value >> 8;
        }

        // Send data over BLE via NUS service. Makes sure not to send more than 20 bytes.
        if((SAADC_SAMPLES_IN_BUFFER*2) <= 20)
        {
            bytes_to_send = (SAADC_SAMPLES_IN_BUFFER*2);
        }
        else
        {
            bytes_to_send = 20;
        }

        m_adc_evt_counter++;
    }
}

void saadc_init(void)
{
    nrf_drv_saadc_init(m_buffer_pool[0],SAADC_SAMPLES_IN_BUFFER, saadc_callback);
    saadc_sampling_event_init();
    saadc_sampling_event_enable();
}

/**
 * @brief Function for main application entry.
 */
int main(void)
{
    uint32_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();


    saadc_init();

#if 0
    /* 2-channel PWM, 200Hz, output on DK LED pins. */
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_2CH(10L, BSP_LED_0, BSP_LED_1);
    /* Switch the polarity of the second channel. */
    pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
    /* Initialize and enable PWM. */
    err_code = app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
    APP_ERROR_CHECK(err_code);
    app_pwm_enable(&PWM1);
    //err_code = nrf_drv_power_init(NULL);
    //APP_ERROR_CHECK(err_code);
#endif
    //ret_code_t ret_code = nrf_pwr_mgmt_init();
    //APP_ERROR_CHECK(ret_code);

    NRF_LOG_INFO("SAADC HAL simple example.");
    uint32_t value;
//    mysaadc_init();
    while (1)
    {
//        value = 10;
//        app_pwm_channel_duty_set(&PWM1, 0, value);
//        value = 50;
//        app_pwm_channel_duty_set(&PWM1, 1, value);
//        //nrf_pwr_mgmt_run();
//        NRF_LOG_INFO("NRF_SAADC_RESISTOR_VDD1_2 : %d",NRF_SAADC_RESISTOR_VDD1_2);
        nrf_delay_ms(100);
        NRF_LOG_FLUSH();
    }
}


/** @} */
