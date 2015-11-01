//-----------------------------------------//
#include "arms_manager.h"

#include "tcp_server.h"

#include <cassert>
//-----------------------------------------//
ArmsManager::ArmsManager(QObject *parent) :
    QObject(parent),
    tcpServer_(std::unique_ptr<TcpServer>(new TcpServer()))
{

}
//-----------------------------------------//
ArmsManager::~ArmsManager()
{

}
//-----------------------------------------//
void ArmsManager::init()
{
    // start tcp server
    assert(tcpServer_);
    connect(tcpServer_.get(),
            SIGNAL(clientConnected(int)),
            this,
            SLOT(onClientConnected(int)));
    tcpServer_->start(2015);
}
//-----------------------------------------//
void ArmsManager::onClientConnected(int handle)
{
    ClientInfo info;
    info.handle = handle;
    info.socket = std::make_shared<QTcpSocket>();
    if(!info.socket->setSocketDescriptor(handle))
    {
        qWarning() << "Error of setSocketDescriptor: "
                   << info.socket->errorString();
        return;
    }
    info.ip = info.socket->peerAddress().toString();

//    connect(info.socket.get(),
//         SIGNAL(readyRead()),
//         this,
//         SLOT(onReceived()),
//         Qt::QueuedConnection);
    connect(info.socket.get(),
         SIGNAL(disconnected()),
         this,
         SLOT(onClientDisconnected()),
         Qt::QueuedConnection);

    assert(sockets_.count(handle) == 0);
    sockets_.emplace(handle, info);

    emit clientConnected(info.ip);
}
//-----------------------------------------//
void ArmsManager::onClientDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    assert(socket);

    for(const auto& item : sockets_)
        if(item.second.socket.get() == socket)
        {
            const ClientInfo& info = item.second;
            emit clientDisconnected(info.ip);
            sockets_.erase(info.handle);
            break;
        }

}
//-----------------------------------------//
