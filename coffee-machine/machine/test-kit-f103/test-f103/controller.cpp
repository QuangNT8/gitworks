#include "controller.h"
#include "valve/controller.h"

kit::Controller kit::CONTROLLER;

kit::Controller::Controller()
{

}

void kit::Controller::init()
{
    initGPIO_();    
}

void kit::Controller::loop()
{

}

