#ifndef CHANGENOTIFYSTATYSINCOMINGCOMMAND_H
#define CHANGENOTIFYSTATYSINCOMINGCOMMAND_H

#include <QObject>
#include <QDateTime>

#include "hardwareclient_global.h"
#include "iincomingcommand.h"

namespace ArmHardwareClient {


class HARDWARE_CLIENTSHARED_EXPORT ChangeNotifyStatysIncomingCommand : public IIncomingCommand
{
    Q_OBJECT
public:
    enum NotifyState {
        Wait = 0x00,
        Process
    };

public:
    explicit ChangeNotifyStatysIncomingCommand(QObject *parent = 0);

    // IIncommingCommand interface
    virtual Type commandType() const override;

    virtual QString hardwareId() const = 0;
    virtual NotifyState notifyState() const = 0;
    virtual QDateTime stateChangeTime() const = 0;
};

}

#endif // CHANGENOTIFYSTATYSINCOMINGCOMMAND_H
