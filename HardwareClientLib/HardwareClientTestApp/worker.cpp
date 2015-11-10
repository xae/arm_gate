#include "worker.h"

#include <QCoreApplication>

Worker::Worker(INotifyContextAccess *notifyContextAccess) :
    mNotifyContextAccess(notifyContextAccess),
    mWorker(new ThreadWorker())
{
    connect(mWorker, SIGNAL(connectedToServer()),
            this, SLOT(onConnectedToServer()), Qt::QueuedConnection);
    connect(mWorker, SIGNAL(disconnectedFromServer()),
            this, SLOT(onDisconnectedFromServer()));
    connect(mWorker, SIGNAL(error()),
            this, SLOT(onError()));
    connect(mWorker, SIGNAL(logMessage(QString)),
            this, SIGNAL(logMessage(QString)));
    mWorker->moveToThread(&mThread);
    mThread.start();

    connect(qApp, SIGNAL(aboutToQuit()),
            this, SLOT(quit()));

}

Worker::~Worker()
{
    mThread.quit();
    mThread.wait();
}

void Worker::connectDisconnect()
{
    mWorker->setServerProperties(mNotifyContextAccess->serverAddress(), mNotifyContextAccess->serverPort());
    if (mWorker->isOpen()) {
        mWorker->close();
    } else {
        mWorker->connectToServer();
    }
}

void Worker::startNotify()
{
    QStringList hardwaresIds = mNotifyContextAccess->hardwaresIds();
    ArmHardwareClient::NotifyContext notifyContext = mNotifyContextAccess->notifyContext();
    mWorker->setNotifyCurrentNotifyContext(hardwaresIds, notifyContext);
    mWorker->startNotify();
}

void Worker::requestStatuses()
{
    QStringList hardwaresIds = mNotifyContextAccess->hardwaresIds();
    mWorker->requsestStatuses(hardwaresIds);
}

void Worker::onConnectedToServer()
{
    emit connectDisconnectButtonText(tr("Disconnect"));
}

void Worker::onDisconnectedFromServer()
{
    emit connectDisconnectButtonText(tr("Connect"));
}

void Worker::onError()
{
    onDisconnectedFromServer();
}

void Worker::quit()
{
    QMetaObject::invokeMethod(mWorker, "deleteLater");
    mThread.quit();
    mThread.wait();
}

