#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

namespace hal {

class GPIO
{
public:
    static void configPin(int32_t pin, uint32_t dir, uint32_t input, uint32_t pull, uint32_t drive, uint32_t sense);
    static void configPinOutput(int32_t pin);
    static void configPinInput(int32_t pin);
    static void pinSet(int32_t pin);
    static void pinClear(int32_t pin);
    static void pinToggle(int32_t pin);
};
}

#endif // GPIO_H
