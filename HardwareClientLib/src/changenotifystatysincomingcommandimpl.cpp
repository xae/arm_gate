#include "changenotifystatysincomingcommandimpl.h"

using namespace ArmHardwareClient;

ChangeNotifyStatysIncomingCommandImpl::ChangeNotifyStatysIncomingCommandImpl(
        const QString &hardwareId,
        const NotifyState &notifyState,
        const QDateTime &stateChangeTime,
        QObject *parent) :
    ChangeNotifyStatysIncomingCommand(parent),
    mHardwareId(hardwareId),
    mNotifyState(notifyState),
    mStateChangeTime(stateChangeTime)
{

}



QString ArmHardwareClient::ChangeNotifyStatysIncomingCommandImpl::hardwareId() const
{
    return mHardwareId;
}

ChangeNotifyStatysIncomingCommand::NotifyState ArmHardwareClient::ChangeNotifyStatysIncomingCommandImpl::notifyState() const
{
    return mNotifyState;
}

QDateTime ArmHardwareClient::ChangeNotifyStatysIncomingCommandImpl::stateChangeTime() const
{
    return mStateChangeTime;
}
