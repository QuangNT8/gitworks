#include "gpio.h"
#include "nrf.h"

void hal::GPIO::configPin(int32_t pin, uint32_t dir, uint32_t input, uint32_t pull, uint32_t drive, uint32_t sense)
{
    NRF_P0->PIN_CNF[pin] = (dir << GPIO_PIN_CNF_DIR_Pos)
            | (input << GPIO_PIN_CNF_INPUT_Pos)
            | (pull << GPIO_PIN_CNF_PULL_Pos)
            | (drive << GPIO_PIN_CNF_DRIVE_Pos)
            | (sense << GPIO_PIN_CNF_SENSE_Pos);
}

void hal::GPIO::configPinOutput(int32_t pin)
{
    NRF_P0->PIN_CNF[pin] = (1UL << GPIO_PIN_CNF_DIR_Pos) | (1UL << GPIO_PIN_CNF_INPUT_Pos);
}

void hal::GPIO::configPinInput(int32_t pin)
{
    NRF_P0->PIN_CNF[pin] = 0; //IN_DIR, INPUT_CONNECT, NO_PULL
}

void hal::GPIO::pinSet(int32_t pin)
{
    NRF_P0->OUTSET = (1UL << pin);
}

void hal::GPIO::pinClear(int32_t pin)
{
    NRF_P0->OUTCLR = (1UL << pin);
    //NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_IN0_Msk;
}

void hal::GPIO::pinToggle(int32_t pin)
{
    uint32_t state = NRF_P0->OUT;

    NRF_P0->OUTSET = (~state & (1UL << pin));
    NRF_P0->OUTCLR = (state & (1UL << pin));
}
