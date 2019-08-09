#ifndef CONTROLLER_H
#define CONTROLLER_H

namespace blink
{

class Controller
{
public:
    void init();
    void loop();
    Controller(){}
private:
    void ledOn_();
    void ledOff_();
};

extern blink::Controller CONTROLLER;

}

#endif // CONTROLLER_H
