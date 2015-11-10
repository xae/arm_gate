#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include "iabstracttransport.h"

#include <QObject>
#include <QPointer>
#include <QtNetwork/QTcpSocket>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace AsyncClient {

class ABSTRACTASYNCCLIENTSHARED_EXPORT TcpTransport : public IAbstractTransport
{
    Q_OBJECT
public:
    TcpTransport(const QString &host, int port, QObject *parent = 0);
    TcpTransport(QTcpSocket *socket, QObject *parent = 0);
    ~TcpTransport();

    // IAbstractTransport interface
public:
    virtual bool open();
    virtual void close();
    virtual bool isOpen() const;
    virtual QByteArray read(const int size);
    virtual QByteArray readAll();
    virtual bool getChar(char *c);
    virtual void write(const QByteArray &data);
    virtual IAbstractTransport::TransportState state() const;
    virtual int timeoutWaitDataValue() const;
    QString peerAddres() const;

private:
    void changeState(TransportState state);
    void connectSocket();

private slots:
    void onSocketError(QAbstractSocket::SocketError error);
    void onConnected();
    void onDisconnected();

private:
    QPointer<QTcpSocket> mTcpSocket;
    const QString mHost;
    const int mPort;
    TransportState mCurrentState;
    const int mTimeoutWaitDateValue;

};

}
#endif // TCPTRANSPORT_H

