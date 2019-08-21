#include "pwm.h"
#include "uart/controller.h"

int main()
{
    uart::CONTROLLER.init();
    pwm::SETTUP.init();

    while (true)
    {
        uart::CONTROLLER.loop();
        pwm::SETTUP.loop();
    }
    return 0;
}
