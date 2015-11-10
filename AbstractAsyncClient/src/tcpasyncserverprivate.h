#ifndef TCPASYNCSERVERPRIVATE
#define TCPASYNCSERVERPRIVATE

#include <QObject>
#include <QTcpServer>

namespace AsyncClient {

class AbstractAsyncClient;
class TcpAsyncServer;
class TcpAsyncServerPrivate : public QObject
{
    Q_OBJECT
public:
    TcpAsyncServerPrivate(const QByteArray &signature,
                          const QHostAddress & address,
                          quint16 port, QObject *parent);
    bool listen();
    void close();
    bool hasPendingConnection() const;
    bool isListening() const;
    AbstractAsyncClient *nextPendingConnection();
    int maxPendingConnections() const;
    void setMaxPendingConnections(int numConnections);

private:
    friend class TcpAsyncServer;
    QTcpServer *mServer;
    const QByteArray mSignature;
    const QHostAddress mHostAddress;
    const quint16 mPort;
};



}
#endif // TCPASYNCSERVERPRIVATE

