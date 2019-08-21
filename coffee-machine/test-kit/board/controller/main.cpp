#include "kit/kit-controller.h"
#include "uart2/controller.h"
#include "uart/controller.h"
#include "valve/controller.h"

int main()
{
    kit::CONTROLLER.initSystem();
    kit::CONTROLLER.init();
    uart::CONTROLLER.init();
    uart2::CONTROLLER.init();
    valve::CONTROLLER.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        uart2::CONTROLLER.loop();
        kit::CONTROLLER.loop();
        valve::CONTROLLER.loop();
    }
    return 0;
}
