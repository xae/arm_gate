#include "changenotifystatysincomingcommand.h"

using namespace ArmHardwareClient;

ChangeNotifyStatysIncomingCommand::ChangeNotifyStatysIncomingCommand(QObject *parent) :
    IIncomingCommand(parent)
{

}

IIncomingCommand::Type ArmHardwareClient::ChangeNotifyStatysIncomingCommand::commandType() const
{
    return NotifyStatusChange;
}
