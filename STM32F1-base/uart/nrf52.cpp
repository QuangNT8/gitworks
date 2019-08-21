#ifdef MCU_NRF52

#define RTS_PIN    5
#define TXD_PIN    6
#define CTS_PIN    7
#define RXD_PIN    8

#include "nrf.h"
#include "controller.h"
#include "hal/gpio.h"

void uart::Controller::initUART_()
{
    NRF_UART0->BAUDRATE = UART_BAUDRATE_BAUDRATE_Baud115200;
    NRF_UART0->CONFIG =0; //no Parity, no hadware flow control

    /*RTS*/
    NRF_UART0->PSELRTS = RTS_PIN;
    hal::GPIO::pinSet(RTS_PIN);
    hal::GPIO::configPinOutput(RTS_PIN);

    /*TXD*/
    NRF_UART0->PSELTXD = TXD_PIN;
    hal::GPIO::pinSet(TXD_PIN);
    hal::GPIO::configPinOutput(TXD_PIN);

    /*CTS*/
    NRF_UART0->PSELCTS = CTS_PIN;
    hal::GPIO::configPinInput(CTS_PIN);

    /*RXD*/
    NRF_UART0->PSELRXD = RXD_PIN;
    hal::GPIO::configPinInput(RXD_PIN);

    /*start uart*/
    NRF_UART0->ENABLE = UART_ENABLE_ENABLE_Enabled;
    NRF_UART0->TASKS_STARTRX = 1UL;
    NRF_UART0->TASKS_STARTTX = 1UL;
    NRF_UART0->TXD = 0;
}

bool uart::Controller::rxReady_()
{
    return NRF_UART0->EVENTS_RXDRDY;
}

bool uart::Controller::txReady_()
{
    return NRF_UART0->EVENTS_TXDRDY;
}

uint8_t uart::Controller::read_()
{
    NRF_UART0->EVENTS_RXDRDY = 0UL;
    return NRF_UART0->RXD;
}

void uart::Controller::write_(uint8_t c)
{
    NRF_UART0->EVENTS_TXDRDY = 0UL;
    NRF_UART0->TXD = c;
}

#endif
