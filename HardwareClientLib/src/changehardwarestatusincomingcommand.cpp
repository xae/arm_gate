#include "changehardwarestatusincomingcommand.h"

using namespace ArmHardwareClient;

ChangeHardwareStatusIncomingCommand::ChangeHardwareStatusIncomingCommand(QObject *parent) :
    IIncomingCommand(parent)
{

}

ArmHardwareClient::IIncomingCommand::Type ArmHardwareClient::ChangeHardwareStatusIncomingCommand::commandType() const
{
    return HardwareStatusChange;
}
