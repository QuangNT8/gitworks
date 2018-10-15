#include "controller.h"
#include "uart/controller.h"

int main()
{
    adc::CONTROLLER.init();
    uart::CONTROLLER.init();

    while (true)
    {
        adc::CONTROLLER.loop();
        uart::CONTROLLER.loop();
    }
    return 0;
}
