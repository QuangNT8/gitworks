#ifndef UART2_CONTROLLER_H_
#define UART2_CONTROLLER_H_
#include <stdint.h>

namespace uart2
{

class Controller
{
public:
    void init();
    void loop();

private:
    void initUART_();
    bool rxReady_();
    bool txReady_();
    uint8_t read_();
    void write_(uint8_t c);

    /*rx*/
    enum RxState{HEADER1 = 0, HEADER2, DATA};
    uint8_t rxBuffer_[16];
    uint8_t rxState_;
    uint8_t rxIndex_;

    void scaleData_();
};

extern uart2::Controller CONTROLLER;

}

#endif // UART2_CONTROLLER_H_
