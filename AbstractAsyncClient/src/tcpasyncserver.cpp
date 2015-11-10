#include "tcpasyncserver.h"
#include "tcptransport.h"
#include "abstractasyncclient.h"
#include "tcpasyncserverprivate.h"


using namespace AsyncClient;

TcpAsyncServerPrivate::TcpAsyncServerPrivate(const QByteArray &signature,
        const QHostAddress &address,
        quint16 port, QObject *parent) :
    QObject(parent),
    mServer(new QTcpServer(this)),
    mSignature(signature),
    mHostAddress(address),
    mPort(port)
{

}

bool TcpAsyncServerPrivate::listen()
{
    return mServer->listen(mHostAddress, mPort);
}

void TcpAsyncServerPrivate::close()
{
    mServer->close();
}

bool TcpAsyncServerPrivate::hasPendingConnection() const
{
    return mServer->hasPendingConnections();
}

bool TcpAsyncServerPrivate::isListening() const
{
    return mServer->isListening();
}

AbstractAsyncClient *TcpAsyncServerPrivate::nextPendingConnection()
{
    //TODO:
    QTcpSocket *socket = mServer->nextPendingConnection();
    if (!socket) {
        return nullptr;
    }
    TcpTransport *transport = new TcpTransport(socket);
    AbstractAsyncClient *client = new AbstractAsyncClient(transport, mSignature, this);
    return client;
}

int TcpAsyncServerPrivate::maxPendingConnections() const
{
    return mServer->maxPendingConnections();
}

void TcpAsyncServerPrivate::setMaxPendingConnections(int numConnections)
{
    mServer->setMaxPendingConnections(numConnections);
}

TcpAsyncServer::TcpAsyncServer(
        const QByteArray &signature,
        const QHostAddress &address,
        quint16 port,
        QObject *parent) :
    QObject(parent),
    d(new TcpAsyncServerPrivate(signature, address, port, this))
{
    connect(d->mServer, SIGNAL(newConnection()), this, SIGNAL(newConnection()));
}

TcpAsyncServer::~TcpAsyncServer()
{
}

bool TcpAsyncServer::listen()
{
    return d->listen();
}

void TcpAsyncServer::close()
{
    d->close();
}

bool TcpAsyncServer::hasPendingConnection() const
{
    return d->hasPendingConnection();
}

bool TcpAsyncServer::isListening() const
{
    return d->isListening();
}

AbstractAsyncClient *TcpAsyncServer::nextPendingConnection()
{
    return d->nextPendingConnection();
}

int TcpAsyncServer::maxPendingConnections() const
{
    return d->maxPendingConnections();
}

void TcpAsyncServer::setMaxPendingConnections(int numConnections)
{
    d->setMaxPendingConnections(numConnections);
}
