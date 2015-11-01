//-----------------------------------------//
#include "tcp_server.h"

#include <QHostAddress>
#include <QDebug>
//-----------------------------------------//
TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{
}
//-----------------------------------------//
TcpServer::~TcpServer()
{
    stop();
}
//-----------------------------------------//
void TcpServer::start(unsigned port)
{
    if(isListening())
        return;

    setMaxPendingConnections(100);
    if(!listen(QHostAddress::Any, port))
        qWarning() << "Error of starting listening port " << port;
}
//-----------------------------------------//
void TcpServer::stop()
{
    if(!isListening())
        return;

    close();
}
//-----------------------------------------//
void TcpServer::incomingConnection(int handle)
{
    emit clientConnected(handle);
}
//-----------------------------------------//
