#include "stm32_timer/timer.h"

int main()
{
    //blink::CONTROLLER.init();
    timer::CONTROLLER.init(1);
    while (true)
    {
        timer::CONTROLLER.loop();
    }
//    return 0;
}
