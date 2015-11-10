#ifndef SIMPLETRANSPORT_H
#define SIMPLETRANSPORT_H

#include "iabstracttransport.h"

class SimpleTransport : public AsyncClient::IAbstractTransport {
    Q_OBJECT

public:
    void setTimeout(const int timeout);

    // IAbstractTransport interface
public:
    SimpleTransport(QObject *parent = 0) : IAbstractTransport(parent), mTimeout(60000) {}
    virtual bool open();
    virtual void close();
    virtual bool isOpen() const;
    virtual QByteArray read(const int size);
    virtual QByteArray readAll();
    virtual bool getChar(char *c);
    virtual void write(const QByteArray &data);
    virtual AsyncClient::IAbstractTransport::TransportState state() const;
    virtual int timeoutWaitDataValue() const;
    QString peerAddres() const;

private:
    QByteArray data;
    int mTimeout;

};
#endif // SIMPLETRANSPORT_H

