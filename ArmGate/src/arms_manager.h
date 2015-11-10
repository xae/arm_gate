//-----------------------------------------//
#pragma once

#include <QObject>
#include <memory>

#include "hardwareserver.h"

//-----------------------------------------//
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
    const std::unique_ptr<ArmHardwareServer::HardwareServer> server_;
};
//-----------------------------------------//

