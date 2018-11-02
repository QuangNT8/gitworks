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
*
* @defgroup nrf_dev_button_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_button_radio_tx_example
*
* @brief Radio Transceiver Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "nrf_delay.h"
#include "radio_config.h"
#include "nrf_gpio.h"

#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nordic_common.h"
#include "nrf_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define LED1_ON          	nrf_gpio_pin_clear(17)
#define LED1_OFF          	nrf_gpio_pin_set(17)
#define LED1_toggle         nrf_gpio_pin_toggle(17)

#define LED2_ON          	nrf_gpio_pin_clear(18)
#define LED2_OFF          	nrf_gpio_pin_set(18)
#define LED2_toggle         nrf_gpio_pin_toggle(18)

#define RELAY1_OFF         	nrf_gpio_pin_clear(12)
#define RELAY1_ON        	nrf_gpio_pin_set(12)
#define RELAY1_toggle      	nrf_gpio_pin_toggle(12)

#define RELAY2_OFF         	nrf_gpio_pin_clear(16)
#define RELAY2_ON        	nrf_gpio_pin_set(16)
#define RELAY2_toggle      	nrf_gpio_pin_toggle(16)

#define SW1_init			nrf_gpio_cfg_input(14, BUTTON_PULL)	 
#define SW2_init			nrf_gpio_cfg_input(13, BUTTON_PULL)	 
#define SW1_read        	nrf_gpio_pin_read(14)
#define SW2_read         	nrf_gpio_pin_read(13)

typedef struct
{
    uint8_t Header;
    uint8_t Count;
    uint8_t DeviceID;
    uint8_t TargetID;
    uint8_t data[8] ;
    uint32_t eof;
} MSG_DATA_t;

static MSG_DATA_t                   packet;                    /**< Packet to transmit. */

void MSG_init()
{
    packet.Header = 0xff;
    packet.DeviceID = 54;
    packet.TargetID = 36;
    packet.eof = 0xffffffff;
}

/**@brief Function for sending packet.
 */
void send_packet()
{
    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
    NRF_RADIO->EVENTS_END  = 0U;
    NRF_RADIO->TASKS_START = 1U;

    while (NRF_RADIO->EVENTS_END == 0U)
    {
        // wait
    }

    //uint32_t err_code = bsp_indication_set(BSP_INDICATE_SENT_OK);
    //NRF_LOG_INFO("The packet was sent");
    //APP_ERROR_CHECK(err_code);

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
}


/**@brief Function for handling bsp events.
 */
void bsp_evt_handler(bsp_event_t evt)
{
    uint32_t prep_packet = 0;
    switch (evt)
    {
        case BSP_EVENT_KEY_0:
            /* Fall through. */
        case BSP_EVENT_KEY_1:
            /* Fall through. */
        case BSP_EVENT_KEY_2:
            /* Fall through. */
        case BSP_EVENT_KEY_3:
            /* Fall through. */
        case BSP_EVENT_KEY_4:
            /* Fall through. */
        case BSP_EVENT_KEY_5:
            /* Fall through. */
        case BSP_EVENT_KEY_6:
            /* Fall through. */
        case BSP_EVENT_KEY_7:
            /* Get actual button state. */
            for (int i = 0; i < BUTTONS_NUMBER; i++)
            {
                prep_packet |= (bsp_board_button_state_get(i) ? (1 << i) : 0);
            }
            break;
        default:
            /* No implementation needed. */
            break;
    }
//    packet[0] = prep_packet;
}


/**@brief Function for initialization oscillators.
 */
void clock_initialization()
{
    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;
	
	//for(i=0;i<100;i++) j++;
	nrf_delay_ms(1);
#if 0
    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }
#endif
    /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;
	//1for(i=0;i<100;i++) j++;
	nrf_delay_ms(1);
#if 0
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }
#endif
}


/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
    uint32_t err_code = NRF_SUCCESS;
	uint8_t sw1_flag=0;
	uint8_t sw2_flag=0;
    uint8_t i;
	 /* Configure board. */
    bsp_board_leds_init();
	
    clock_initialization();
#if 1
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    err_code = bsp_init(BSP_INIT_LED | BSP_INIT_BUTTONS, bsp_evt_handler);
    APP_ERROR_CHECK(err_code);

	SW1_init;
	SW2_init;
    // Set radio configuration parameters
    radio_configure();

    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)&packet;

    //err_code = bsp_indication_set(BSP_INDICATE_USER_STATE_OFF);
//    NRF_LOG_INFO("Press Any Button");
    APP_ERROR_CHECK(err_code);
#endif
    MSG_init();
    for(i=0;i<8;i++) packet.data[i] = 0;
    while (true)
    {
//        if (packet != 0)
//        {
//            send_packet();
//            NRF_LOG_INFO("The contents of the package was %u", (unsigned int)packet);
//            packet = 0;
//        }
		if(SW1_read==0)
		{
			nrf_delay_ms(50);
			if((SW1_read==0)&&(sw1_flag==0))
			{
                packet.data[0]=1;
                packet.data[1]=2;
                packet.data[2]=3;
                packet.data[3]=4;
                packet.data[4]=5;
                packet.data[5]=6;
                packet.data[6]=7;
                packet.data[7]=8;
				send_packet();
                LED1_toggle;
				//RELAY1_toggle;
                sw1_flag = 1;
			}
		}
		else
		{
			sw1_flag = 0;
		}
		if(SW2_read==0)
		{
			nrf_delay_ms(50);
			if((SW2_read==0)&&(sw2_flag==0))
			{
                LED2_toggle;
				//RELAY2_toggle;
                packet.data[0]=9;
                packet.data[1]=10;
                packet.data[2]=11;
                packet.data[3]=12;
                packet.data[4]=13;
                packet.data[5]=14;
                packet.data[6]=15;
                packet.data[7]=16;
                send_packet();
				sw2_flag = 1;
			}
		}
		else
        {
            sw2_flag = 0;
		}
		
		//packet++;
		//if(packet>=0xffffffff)packet = 0;
		//send_packet();
        //NRF_LOG_INFO("The contents of the package was %u", (unsigned int)packet);
        //NRF_LOG_FLUSH();
		//bsp_board_led_invert(1);
		//nrf_delay_ms(500);
        //__WFE();
    }
}


/**
 *@}
 **/
