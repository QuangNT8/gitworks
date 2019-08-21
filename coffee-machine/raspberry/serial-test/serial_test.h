#ifndef TEST_CONTROLLER_H
#define TEST_CONTROLLER_H

#include "engine.h"
#include "serial/controller.h"

COMPONENT(test, Controller)
public:
    Controller(){}
    void start()
    {       
        sleep(3);
        auto connection =  serial::Controller::instance()->getConnection("/dev/ttyUSB0");
        if (connection != nullptr)
        {
            connection->sendMessage(2, 4, (const uint8_t*) "hihi");
        }
    }
    void stop(){}


COMPONENT_END

#endif // TEST_CONTROLLER_H
