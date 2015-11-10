#include "messageincomingcommand.h"

using namespace ArmHardwareClient;

MessageIncomingCommand::MessageIncomingCommand(QObject *parent) : IIncomingCommand(parent)
{

}


ArmHardwareClient::IIncomingCommand::Type ArmHardwareClient::MessageIncomingCommand::commandType() const
{
    return Message;
}
