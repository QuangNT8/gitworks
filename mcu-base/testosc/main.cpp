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
        foc::CONTROLLER.loopTest();
        plot::CONTROLLER.loop();
    }
    return 0;
}
