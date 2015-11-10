#include "simpletransport.h"

int SimpleTransport::timeoutWaitDataValue() const
{
    return mTimeout;
}

void SimpleTransport::setTimeout(const int timeout)
{
    mTimeout = timeout;
}

bool SimpleTransport::open()
{
    return true;
}

void SimpleTransport::close()
{

}

bool SimpleTransport::isOpen() const
{
    return true;
}

QByteArray SimpleTransport::read(const int size)
{
    QByteArray res = data.left(size);
    int bytesToRemove = data.size() < size ? data.size() : size;
    data.remove(0, bytesToRemove);
    return res;
}

QByteArray SimpleTransport::readAll()
{
    QByteArray tmp = data;
    data.clear();
    return tmp;
}

bool SimpleTransport::getChar(char *c)
{
    if (data.isEmpty()) {
        return false;
    }
    *c = data.at(0);
    data.remove(0, 1);
    return true;
}

void SimpleTransport::write(const QByteArray &data)
{
    this->data.append(data);
    emit readyRead();
}

AsyncClient::IAbstractTransport::TransportState SimpleTransport::state() const
{
    return AsyncClient::IAbstractTransport::Connected;
}


QString SimpleTransport::peerAddres() const
{
    return QString();
}
