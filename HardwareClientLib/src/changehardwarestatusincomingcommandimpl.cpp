#include "changehardwarestatusincomingcommandimpl.h"

using namespace ArmHardwareClient;

ChangeHardwareStatusIncomingCommandImpl::ChangeHardwareStatusIncomingCommandImpl(
        const QString &hardwareId,
        const ChangeHardwareStatusIncomingCommand::HardwareState &hardwareState,
        const QDateTime &stateChangeTime,
        QObject *parent) :
    ChangeHardwareStatusIncomingCommand(parent),
    mHardwareId(hardwareId),
    mHardwareState(hardwareState),
    mStateChangeTime(stateChangeTime)
{

}

QString ChangeHardwareStatusIncomingCommandImpl::hardwareId() const
{
    return mHardwareId;
}

ChangeHardwareStatusIncomingCommand::HardwareState ArmHardwareClient::ChangeHardwareStatusIncomingCommandImpl::hardwareState() const
{
    return mHardwareState;
}

QDateTime ChangeHardwareStatusIncomingCommandImpl::stateChangeTime() const
{
    return mStateChangeTime;
}
