#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QThread>

#include "hardwareinfo.h"
#include "serverthreadworker.h"

namespace HardwareServerTestApp {

class Server : public QObject
{
    Q_OBJECT
public:
    Server(const quint16 port, QObject *parent = 0);
    ~Server();

signals:
    void message(const QString &globalId, const QString &message);
    void notifyStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState);
    void hardwareStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState);
    void newHardwareSetting(const HardwareServerTestApp::HardwareInfo &hardwareSetting);
    void removeHardware(const HardwareServerTestApp::HardwareInfo &hardwareSetting);
    void logMessage(const QString &message);

private:
    QThread mWorkerThread;
    ServerThreadWorker *mWorker;


};

}

#endif // SERVER_H
