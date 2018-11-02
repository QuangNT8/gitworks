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
 * @defgroup uart_example_main main.c
 * @{
 * @ingroup uart_example
 * @brief UART Example Application main file.
 *
 * This file contains the source code for a sample application using UART.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"

#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"

#include "radio_config.h"

#include "nrf_uart.h"


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

//#define debug_enable

#if defined(debug_enable)

#include "app_uart.h"
#include "nrf_uart.h"

/* When UART is used for communication with the host do not use flow control.*/
#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED
#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */

#endif
static uint32_t                   packet;              /**< Packet to transmit. */

uint8_t sw1_flag=0;
uint8_t sw2_flag=0;

void button_scan()
{
	if(SW1_read==0)
	{
		nrf_delay_ms(50);
        if((SW1_read==0)&&(sw1_flag==0))
		{
			LED2_toggle;
			//LED1_toggle;
			RELAY1_toggle;
			sw1_flag = 1;
		}
	}
	else
	{
		sw1_flag = 0;
	}
#if 1	
	if(SW2_read==0)
	{
		nrf_delay_ms(50);
		if((SW2_read==0)&&(sw2_flag==0))
		{
			LED2_toggle;
			RELAY2_toggle;
			sw2_flag = 1;
		}
	}
	else
	{
		sw2_flag = 0;
	}
#endif
}

void button_scan_wallsocket()
{
	if(SW1_read==0)
	{
		nrf_delay_ms(50);
		if((SW1_read==0)&&(sw1_flag==0))
		{
			LED2_toggle;
			RELAY1_toggle;
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
			LED1_toggle;
			RELAY2_toggle;
			sw2_flag = 1;
		}
		sw2_flag = 1;
	}
	else
	{
        sw2_flag = 0;
	}
}



/**@brief Function for reading packet.
 */
uint32_t read_packet()
{
    uint32_t result = 0;

    NRF_RADIO->EVENTS_READY = 0U;
    // Enable radio and wait for ready
    NRF_RADIO->TASKS_RXEN = 1U;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
    NRF_RADIO->EVENTS_END = 0U;
    // Start listening and wait for address received event
    NRF_RADIO->TASKS_START = 1U;

    // Wait for end of packet or buttons state changed
    while (NRF_RADIO->EVENTS_END == 0U)
    {
        //printf("\r\n Wait for end of packet \r\n");
		button_scan_wallsocket();
	}

    if (NRF_RADIO->CRCSTATUS == 1U)
    {
        result = packet;
    }
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
    return result;
}

void clock_initialization()
{
    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;

    /* Wait for the external oscillator to start up */
    nrf_delay_ms(1);
	#if 0
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }
	#endif
    /* Start low frequency crystal oscillator for app_timer(used by bsp)*/
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;
    nrf_delay_ms(1);
	#if 0
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        // Do nothing.
    }
	#endif
}

#if defined(debug_enable)

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void uart_init()
{
	uint32_t err_code;
	
	
	const app_uart_comm_params_t comm_params =
      {
          RX_PIN_NUMBER,
          TX_PIN_NUMBER,
          RTS_PIN_NUMBER,
          CTS_PIN_NUMBER,
          APP_UART_FLOW_CONTROL_DISABLED,
          false,
          NRF_UART_BAUDRATE_115200
      };

    APP_UART_FIFO_INIT(&comm_params,
                         UART_RX_BUF_SIZE,
                         UART_TX_BUF_SIZE,
                         uart_error_handle,
                         APP_IRQ_PRIORITY_LOWEST,
                         err_code);
     APP_ERROR_CHECK(err_code);
}
#endif

void button_remote_wallsocket()
{
	if(packet!=0) 
	{
		if(packet==0x01)
		{
			LED1_toggle;
			RELAY2_toggle;
		}
		if(packet==0x02)
		{
			LED2_toggle;
			RELAY1_toggle;
		}
		packet=0;
	}
}

/**
 * @brief Function for main application entry.
 */
int main(void)
{
    //bsp_board_leds_init();

	
    clock_initialization();
    
    SW1_init;
    SW2_init;
    nrf_gpio_cfg_output(17);
    nrf_gpio_cfg_output(16);
    nrf_gpio_cfg_output(12);
    nrf_gpio_cfg_output(18);
	
	
    radio_configure();
    NRF_RADIO->PACKETPTR = (uint32_t)&packet;
	
	// Set radio configuration parameters

#if defined(debug_enable)
    uart_init();
	printf("\r\n Debug Enable \r\n");
#endif
	packet=0;
    while (true)
    {
		#if defined(debug_enable)
            printf("\r\n RF packet: 0x%lx \r\n",packet);
		#endif
        button_remote_wallsocket();
        read_packet();
//        LED2_toggle;
//        nrf_delay_ms(100);
    }
}


/** @} */
