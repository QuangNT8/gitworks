#ifndef SPI_H
#define SPI_H

#include "stm32f0xx.h"

namespace spi
{
    class Controller
    {
    public:
        enum Command{NONE=0, PLOT_DATA, READ_ANGLE, RUN_PWM, AS_MAGNITUDE};
        void spiReady();
        void init_it();
        void init();
        uint16_t spiRead();
        void spiWrite(uint16_t command);
        uint16_t spi_WriteRead_byte (uint16_t wr);
        uint32_t loop();
        uint16_t SPI_DATA;
        void readAS5047_(uint16_t command);
        void ledtestinit();
    private:
        uint8_t command_ = NONE;
        uint16_t angle_;
        uint16_t magnitude_;
    };

extern spi::Controller CONTROLLER;

}
#endif // CONTROLLER_H
