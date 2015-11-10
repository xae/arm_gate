#ifndef CHANGEHARDWARESTATUSINCOMINGCOMMAND_H
#define CHANGEHARDWARESTATUSINCOMINGCOMMAND_H

#include "hardwareclient_global.h"
#include "iincomingcommand.h"

namespace ArmHardwareClient {


class HARDWARE_CLIENTSHARED_EXPORT ChangeHardwareStatusIncomingCommand : public IIncomingCommand
{
    Q_OBJECT
public:
    enum HardwareState {
        Ok,
        Error
    };

public:
    ChangeHardwareStatusIncomingCommand(QObject *parent = 0);

    // IIncomingCommand interface
public:
    virtual Type commandType() const override;

    virtual QString hardwareId() const = 0;
    virtual HardwareState hardwareState() const = 0;
    virtual QDateTime stateChangeTime() const = 0;
};

}

#endif // CHANGEHARDWARESTATUSINCOMINGCOMMAND_H
