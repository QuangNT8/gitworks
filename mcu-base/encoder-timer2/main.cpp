#include "encoder.h"
#include "uart/controller.h"

int main()
{
    uart::CONTROLLER.init();
    encoder::SETTUP.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        encoder::SETTUP.loop();
    }
    return 0;
}
