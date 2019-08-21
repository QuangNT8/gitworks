#include "uart/controller.h"
#include "motor/controller.h"
#include "motor/command.h"
#include "controller.h"
#include "valve/controller.h"

int main()
{
    uart::CONTROLLER.init();
    plot::CONTROLLER.init();    
    machine::CONTROLLER.init();
    valve::CONTROLLER.init();
    motor::CONTROLLER.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        plot::CONTROLLER.loop();
        motor::CONTROLLER.loop();
        machine::CONTROLLER.loop();
        valve::CONTROLLER.loop();
    }
    return 0;
}
