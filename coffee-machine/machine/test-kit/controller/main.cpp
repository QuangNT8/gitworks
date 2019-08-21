#include "uart/controller.h"
#include "command.h"
#include "controller.h"
#include "valve/controller.h"

int main()
{
    uart::CONTROLLER.init();
    plot::CONTROLLER.init();
    testkit::CONTROLLER.init();
    valve::CONTROLLER.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        plot::CONTROLLER.loop();
        testkit::CONTROLLER.loop();
        valve::CONTROLLER.loop();
    }
    return 0;
}
