#ifndef SERVERTHREADWORKER_H
#define SERVERTHREADWORKER_H

#include <QObject>
#include <QHash>

#include <hardwareserver.h>

#include "hardwareinfo.h"

namespace HardwareServerTestApp {

class EndNotifyCommandHelper : public QObject
{
    Q_OBJECT
public:
    EndNotifyCommandHelper(ArmHardwareServer::HardwareServer *server,
                           const QString &hardwareId,
                           QObject *parent = 0);

public Q_SLOTS:
    void endNotify();

private:
    ArmHardwareServer::HardwareServer *mServer;
    const QString mHardwareId;

};

class ServerThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerThreadWorker(const quint16 port, QObject *parent = 0);

    Q_INVOKABLE void start();

signals:
    void logMessage(const QString &message);

public slots:
    void onMessage(const QString &globalId, const QString &message);
    void onNotifyStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState);
    void onHardwareStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState);
    void onNewHardwareSetting(const HardwareServerTestApp::HardwareInfo &hardwareSetting);
    void onRemoveHardware(const HardwareServerTestApp::HardwareInfo &hardwareSetting);

    void onIncommingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);

private:
    void handleStartNotifyCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);
    void handleRequestHardwareStatusIncomingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);

private:
    ArmHardwareServer::HardwareServer *mServer;
    QHash<QString, HardwareInfo> mHardwares;
    const quint16 mPort;

};

}
#endif // SERVERTHREADWORKER_H
