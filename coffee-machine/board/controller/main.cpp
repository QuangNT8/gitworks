#include "uart/controller.h"
#include "motor/controller.h"
#include "valve/controller.h"

int main()
{
    uart::CONTROLLER.init();
    motor::CONTROLLER.init();
    valve::CONTROLLER.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        motor::CONTROLLER.loop();
        valve::CONTROLLER.loop();
    }
    return 0;
}
