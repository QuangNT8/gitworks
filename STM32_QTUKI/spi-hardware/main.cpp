#include "spihardware.h"
#include "uart/controller.h"

int main()
{
    uart::CONTROLLER.init();
    spi::SETTUP.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        spi::SETTUP.loop();
    }
    return 0;
}
