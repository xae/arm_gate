#ifndef TCPASYNCSERVER_H
#define TCPASYNCSERVER_H
#include "abstractasyncclient_global.h"

#include <QObject>
#include <QHostAddress>

#include <QTcpServer>

namespace AsyncClient {

class AbstractAsyncClient;
class TcpAsyncServerPrivate;

class ABSTRACTASYNCCLIENTSHARED_EXPORT TcpAsyncServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpAsyncServer(const QByteArray &signature,
                            const QHostAddress & address = QHostAddress::Any,
                            quint16 port = 0,
                            QObject *parent = 0);
    ~TcpAsyncServer();

    bool listen();
    void close();
    bool hasPendingConnection() const;
    bool isListening() const;
    AbstractAsyncClient *nextPendingConnection();
    int maxPendingConnections() const;
    void setMaxPendingConnections(int numConnections);

signals:
    void newConnection();

private:
    friend class TcpAsyncServerPrivate;
    TcpAsyncServerPrivate *d;
};

}
#endif // TCPASYNCSERVER_H
