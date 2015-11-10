#ifndef CHANGEHARDWARESTATUSINCOMINGCOMMANDIMPL_H
#define CHANGEHARDWARESTATUSINCOMINGCOMMANDIMPL_H

#include "changehardwarestatusincomingcommand.h"

#include <QDateTime>

namespace ArmHardwareClient {

class ChangeHardwareStatusIncomingCommandImpl : public ChangeHardwareStatusIncomingCommand
{
public:
    ChangeHardwareStatusIncomingCommandImpl(const QString &hardwareId,
                                            const HardwareState &hardwareState,
                                            const QDateTime &stateChangeTime,
                                            QObject *parent = 0);

    // ChangeHardwareStatusIncomingCommand interface
public:
    virtual QString hardwareId() const override;
    virtual HardwareState hardwareState() const override;
    virtual QDateTime stateChangeTime() const override;

private:
    const QString mHardwareId;
    const HardwareState mHardwareState;
    const QDateTime mStateChangeTime;
};


}

#endif // CHANGEHARDWARESTATUSINCOMINGCOMMANDIMPL_H
