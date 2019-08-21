#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H
#include "core/component.h"
#include <QtSerialPort>
#include "core/define.h"

#define HEADER_INDICATOR	0xFE
#define FOOTER_INDICATOR	0xFD
#define MAX_PACKET_LENGTH	250
namespace serial {class Reader;}

COMPONENT(serial,Controller)
    Q_OBJECT
    PROPERTY(quint32, freq)
    PROPERTY(quint32, bandwidth)
public:
    void init();
    void stop();

    Q_INVOKABLE bool open(QString port);
    Q_INVOKABLE void close();
    Q_INVOKABLE QVariant ports();
    Q_INVOKABLE bool isOpen();
    void send(int type, int length, uint8_t* data);
    void sendPacket(int length, uint8_t* data);
    void readData();
    void incFreq(){freq_++;}
private:
    QSerialPort serial_;
    void processCommand_();
    void readyRead_();
    void decode_(uint8_t c);
    void updateTimer_();

    enum RxState{HEADER = 0, LENGTH, TYPE1, TYPE2, DATA, FOOTER};
    uint8_t* rxBuffer_;
    uint8_t rxLength_;
    uint8_t rxState_;
    uint8_t rxIndex_;
    uint32_t rxType_;
    Reader* reader_;
    QTimer *timer;
    quint32 freq_;
    quint32 bandwidth_;
COMPONENT_END

#define SERIAL_COMMAND_BEGIN            void serial::Controller::processCommand_(){switch (rxType_){
#define COMMAND_REG(type, command)      case type: serial_cmd_##command(rxLength_, rxBuffer_); break;
#define SERIAL_COMMAND_END              default:break;}}
#define SERIAL_COMMAND(command)         void serial_cmd_##command(uint8_t length, uint8_t* data)

#endif
