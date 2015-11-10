#include "serverrunner.h"

#include <QApplication>

ServerRunner::ServerRunner(const quint16 port, QObject *parent) :
    QObject(parent),
    mServer(nullptr),
    mPort(port)
{
    connect(qApp, &QApplication::aboutToQuit,
            this, &ServerRunner::onAboutToQuit);
}

void ServerRunner::runServer()
{
    if (thread() != QThread::currentThread()) {
        QMetaObject::invokeMethod(this, "runServer", Qt::QueuedConnection);
    } else {
        mServer = new ServerImpl(mPort);
        connect(mServer, &ServerImpl::serverStatus,
                this, &ServerRunner::serverStatus);
        mServer->listen();


    }
}

void ServerRunner::onAboutToQuit()
{
    if (mServer) {
        delete mServer;
    }
}
