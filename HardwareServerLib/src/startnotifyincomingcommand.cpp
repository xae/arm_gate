#include "startnotifyincomingcommand.h"

using namespace ArmHardwareServer;

StartNotifyIncomingCommand::StartNotifyIncomingCommand(QObject *parent) :
    IIncomingCommand(parent)
{

}

IIncomingCommand::Type ArmHardwareServer::StartNotifyIncomingCommand::commandCode() const
{
    return StartNotify;
}
