#include "messageincomingcommandimpl.h"

using namespace ArmHardwareClient;

MessageIncomingCommandImpl::MessageIncomingCommandImpl(
        const QString &hardwareId,
        const QString &message,
        QObject *parent) :
    MessageIncomingCommand(parent),
    mHardwareId(hardwareId),
    mMessage(message)
{

}



QString ArmHardwareClient::MessageIncomingCommandImpl::hardwareId() const
{
    return mHardwareId;
}

QString ArmHardwareClient::MessageIncomingCommandImpl::message() const
{
    return mMessage;
}
