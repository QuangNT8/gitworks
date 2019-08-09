#include "controller.h"
#include "uart/controller.h"
#include "command.h"

int main()
{
    uart::CONTROLLER.init();
    plot::CONTROLLER.init();
    foc::CONTROLLER.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        plot::CONTROLLER.loop();
        foc::CONTROLLER.loopTest();
        foc::CONTROLLER.loop();
    }
    return 0;
}
