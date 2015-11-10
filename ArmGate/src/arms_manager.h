//-----------------------------------------//
#pragma once

#include <QObject>
#include <memory>

#include "hardwareserver.h"

//-----------------------------------------//
struct HardwareInfo
{
    enum HardwareState {
        Ok,
        Error
    };

    enum NotifyState {
        Wait,
        Process
    };

    QString globalId;
    QString hardwareId;
    QString description;
    QString message;
    int notifyDuration;
    HardwareState hardwareState;
    NotifyState notifyState;
};

class ArmsManager : public QObject
{
    Q_OBJECT
public:
    explicit ArmsManager(QObject *parent = 0);
    ~ArmsManager();

public:
    void init();

private slots:
    void onRecievedCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);

private:
    void handleStartNotifyCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);
    void handleRequestHardwareStatusIncomingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);

private:
    const std::unique_ptr<ArmHardwareServer::HardwareServer> server_;
};
//-----------------------------------------//

