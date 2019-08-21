#include "controller.h"
#include "core/views.h"
#include "serialReader.h"
#include <QTimer>

void serial::Controller::init()
{
    reader_ = new Reader();
    connect(&serial_, &QSerialPort::readyRead, this, &Controller::readyRead_);
    core::Views::instance()->registerComponent("SerialPort", this);
    rxState_ = HEADER;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &serial::Controller::updateTimer_);
    timer->start(1000);

    freq = 0;
    bandwidth =0;
}

void serial::Controller::updateTimer_()
{
    this->set_freq(freq_);
    this->set_bandwidth(bandwidth_);
    freq_ = 0;
    bandwidth_ = 0;
}

QVariant serial::Controller::ports()
{
    auto list = QSerialPortInfo::availablePorts();
    QStringList ret;
    for (auto p: list)
    {
        ret.append(p.portName() + "[" + p.serialNumber() + "]");
    }
    return QVariant::fromValue(ret);
}

bool serial::Controller::open(QString port)
{
    QStringList list = port.split("[");

    qDebug()<<list[0];

    if (serial_.isOpen()) serial_.close();
    serial_.setBaudRate(QSerialPort::Baud115200);
    serial_.setStopBits(QSerialPort::OneStop);
    serial_.setPortName(list[0]);
    if (serial_.open(QIODevice::ReadWrite))
    {
        qDebug()<<"Serial Port opened";
        return true;
    }
    else
    {
        qDebug()<<"Serial Port Error:"<<serial_.errorString();
        return false;
    }
}
bool serial::Controller::isOpen()
{
    return serial_.isOpen();
}

void serial::Controller::close()
{
    if (serial_.isOpen())
    {
        serial_.close();
        qDebug()<<"Serial Port closed";
    }
}

void serial::Controller::stop()
{
    this->close();
}

void serial::Controller::readyRead_()
{
    //reader_->beginRead();
    auto buf = serial_.readAll();
    for (auto c: buf)
    {
        decode_(c);
    }
}

void serial::Controller::readData()
{
    while (serial_.bytesAvailable() > 0 )
    {
        auto buf = serial_.readAll();
        for (auto c: buf)
        {
            decode_(c);
        }
    }
}

void serial::Controller::decode_(uint8_t c)
{
    bandwidth_++;
    switch (rxState_)
    {
    case HEADER:
        if (c == HEADER_INDICATOR) rxState_ = LENGTH;
        break;
    case LENGTH:
        if (c > MAX_PACKET_LENGTH) rxState_ = HEADER;
        else
        {
            rxLength_ = c;
            rxIndex_ = 0;
            rxState_ = TYPE1;
        }
        break;
    case TYPE1:
        rxType_ = c;
        rxState_ = TYPE2;
        break;
    case TYPE2:
        rxType_ = (rxType_<<8) + c;
        if (rxLength_>0)
        {
            rxBuffer_ = new uint8_t[rxLength_];
            rxState_ = DATA;
        }
        else rxState_ = FOOTER;
        break;
    case DATA:
        rxBuffer_[rxIndex_] = c;
        rxIndex_++;
        if (rxIndex_ == rxLength_) rxState_ = FOOTER;
        break;
    case FOOTER:
        if (c == FOOTER_INDICATOR)
        {
            this->processCommand_();
            delete[] rxBuffer_;
        }
        rxState_ = HEADER;
        break;
    default:
        rxState_ = HEADER;
        break;
    }
}

void serial::Controller::sendPacket(int length, uint8_t* data)
{
    serial_.write((char*)data, length);
}

void serial::Controller::send(int type, int length, uint8_t* data)
{
    uint8_t buf[length + 5];
    buf[0] = HEADER_INDICATOR;
    buf[1] = length;
    buf[2] = ((type >> 8) & 0xFF);
    buf[3] = (type & 0xFF);
    buf[length+4] = FOOTER_INDICATOR;

    for (int i=0;i<length; i++)
    {
        buf[i+4] = data[i];
    }
    serial_.write((char*)buf, length + 5);
}
