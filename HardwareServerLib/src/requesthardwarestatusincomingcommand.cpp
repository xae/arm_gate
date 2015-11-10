#include "requesthardwarestatusincomingcommand.h"

using namespace ArmHardwareServer;

RequestHardwareStatusIncomingCommand::RequestHardwareStatusIncomingCommand(QObject *parent) :
    IIncomingCommand(parent)
{

}

RequestHardwareStatusIncomingCommand::Type ArmHardwareServer::RequestHardwareStatusIncomingCommand::commandCode() const
{
    return RequestStatuses;
}
