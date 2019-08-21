#include <QDateTime>
#include <QtEndian>
#include "tcpController.h"

void tcp::Controller::init()
{
    state = DISCONNECTED;
    message_.state = Message::READ_HEADER;

    socket_ = new QTcpSocket(this);
    sequence_ = QDateTime::currentMSecsSinceEpoch();

    connect(socket_, &QTcpSocket::disconnected, this, [this]
    {
        set_state(DISCONNECTED);
    });

    connect(socket_, &QTcpSocket::connected, this, [this]
    {
        set_state(CONNECTED);
    });

    connect(socket_, &QTcpSocket::readyRead, this, &tcp::Controller::readyRead_);
    connect(this, &Controller::sendMessage, this, &Controller::sendMessage_);
}

void tcp::Controller::start()
{
}

void tcp::Controller::stop()
{
    this->close();
}

void tcp::Controller::readyRead_()
{
    while (1)
    {
        switch (message_.state)
        {
            case Message::READ_HEADER:
            {
                if (socket_->bytesAvailable()<PACKET_HEADER_SIZE) return;
                socket_->read((char*)message_.header,PACKET_HEADER_SIZE);

                if (message_.header[0] != PACKET_MAGIC || message_.header[1] != PACKET_MAGIC) qDebug()<< "Magic bytes are not matched";
                message_.size = (message_.header[2] << 16) + (message_.header[3]<<8) + message_.header[4]; // 3 bytes for size
                message_.type = (message_.header[5] << 8) + message_.header[6];//2 bytes for type

                if ((message_.header[7] & 0x02) == 2)
                {
                    message_.state = Message::READ_ID;
                    message_.size -= 16;
                }
                else
                {
                    message_.state = Message::READ_DATA;
                    message_.size -= 8;
                    message_.id = 0;
                }
                message_.data = new char[message_.size];
                break;
            }
            case Message::READ_ID:
            {
                if (socket_->bytesAvailable()<8) return;
                socket_->read((char*)message_.header,8);
                message_.id = qFromBigEndian<qint64>(message_.header);
                message_.state = Message::READ_DATA;
                break;
            }
            case Message::READ_DATA:
            {
                if (socket_->bytesAvailable()<message_.size) return;
                socket_->read(message_.data,message_.size);

                if (message_.id == 0)
                {
                    Event* e = create_(message_.type);
                    if (e!=nullptr)
                    {
                        e->setData(message_.data,message_.size);
                        e->fire();
                    }
                    else delete[] message_.data;
                }
                else
                {
                    auto iter = operations_.find(message_.id);
                    if(iter != operations_.end())
                    {
                        auto op = iter->second;
                        op->reply_ = message_.data;
                        op->replySize_ = message_.size;
                        operations_.erase(iter);
                        op->fire();
                    }
                    else delete[] message_.data;
                }

                lastReceived_ = QDateTime::currentMSecsSinceEpoch();
                message_.state = Message::READ_HEADER;
                break;
            }
        }
    }
}

void tcp::Controller::open(QString host, int port)
{
    if(socket_->state() != QAbstractSocket::ConnectedState)
    {
        socket_->connectToHost(host,port);
        set_state(CONNECTING);
    }
}

void tcp::Controller::close()
{
    for (auto& i: operations_)
    {
        i.second->deleteLater();
    }
    operations_.clear();
    if (state != DISCONNECTED)
    {
        socket_->disconnectFromHost();
        set_state(DISCONNECTING);
    }
}

uint64_t tcp::Controller::nextId()
{
     QMutexLocker lock(&mutex_);
    return sequence_++;
}

void tcp::Controller::sendMessage_(Operation* op)
{
    if (socket_->state() == QAbstractSocket::ConnectedState)
    {
        socket_->write(op->request_, op->requestSize_);
        operations_[op->id_] = op;
    }
}

void tcp::Controller::registerEvent(uint16_t type, core::Creator<tcp::Event>* creator)
{
    events_[type] = creator;
}

tcp::Event* tcp::Controller::create_(uint16_t type)
{
    auto it = events_.find(type);
    if (it != events_.end())
    {
        return  it.value()->create();
    }
    qDebug()<<"No operation type:"<<type;
    return nullptr;
}

