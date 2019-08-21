#include "engine.h"
#include "serial/controller.h"
#include "serial-test/serial_test.h"

int main()
{
    auto engine = core::Engine::instance();
    engine->registerComponent<test::Controller>();
    engine->registerComponent<serial::Controller>();
    engine->start();
    return 0;
}
