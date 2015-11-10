#include "hardwareserver.h"
#include "hardwareserverprivate.h"

using namespace ArmHardwareServer;

HardwareServer::HardwareServer(const quint16 port, const QHostAddress &address, QObject *parent) :
    QObject(parent),
    d(new HardwareServerPrivate(this, port, address, this))
{
    qRegisterMetaType<ArmHardwareServer::IIncomingCommandSharedPtr>("ArmHardwareServer::IIncomingCommandSharedPtr");
}

HardwareServer::~HardwareServer()
{

}

bool HardwareServer::startListen()
{
    return d->startListen();
}

void HardwareServer::close()
{
    d->close();
}

void HardwareServer::sendOutgoingCommand(IOutgoingCommand *cmd)
{
    d->sendOutgoingCommand(cmd);
}
