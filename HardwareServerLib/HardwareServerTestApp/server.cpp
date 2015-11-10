#include "server.h"

using namespace HardwareServerTestApp;

Server::Server(const quint16 port, QObject *parent) :
    QObject(parent),
    mWorker(new ServerThreadWorker(port))
{
    connect(this, SIGNAL(hardwareStateChange(QString,HardwareServerTestApp::HardwareInfo::HardwareState)),
            mWorker, SLOT(onHardwareStateChange(QString,HardwareServerTestApp::HardwareInfo::HardwareState)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(message(QString,QString)),
            mWorker, SLOT(onMessage(QString,QString)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(newHardwareSetting(HardwareServerTestApp::HardwareInfo)),
            mWorker, SLOT(onNewHardwareSetting(HardwareServerTestApp::HardwareInfo)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(notifyStateChange(QString,HardwareServerTestApp::HardwareInfo::NotifyState)),
            mWorker, SLOT(onNotifyStateChange(QString,HardwareServerTestApp::HardwareInfo::NotifyState)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(removeHardware(HardwareServerTestApp::HardwareInfo)),
            mWorker, SLOT(onRemoveHardware(HardwareServerTestApp::HardwareInfo)),
            Qt::QueuedConnection);
    connect(&mWorkerThread, SIGNAL(finished()),
            mWorker, SLOT(deleteLater()),
            Qt::QueuedConnection);
    connect(mWorker, SIGNAL(logMessage(QString)),
            this, SIGNAL(logMessage(QString)),
            Qt::QueuedConnection);

    mWorker->moveToThread(&mWorkerThread);
    mWorkerThread.start();
    mWorker->start();
}

Server::~Server()
{
    mWorkerThread.quit();
    mWorkerThread.wait();
}

