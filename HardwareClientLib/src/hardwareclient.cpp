#include "hardwareclient.h"

#include "hardwareclientprivate.h"

#include <QMetaType>

using namespace ArmHardwareClient;

HardwareClient::HardwareClient(const QHostAddress &address, const quint16 port, QObject *parent) :
    QObject(parent),
    d(new HardwareClientPrivate(this, address, port, this))
{
    qRegisterMetaType<IIncomingCommandSharedPtr>("ArmHardwareClient::IIncomingCommandSharedPtr");
    qRegisterMetaType<ArmHardwareClient::IOutgoingCommandSharedPtr>("ArmHardwareClient::IOutgoingCommandSharedPtr");
}

HardwareClient::~HardwareClient()
{
    if (isOpen()) {
        close();
    }
}

QString HardwareClient::serverAddress() const
{
    return d->serverAddress();
}

void HardwareClient::sendOutgoingCommand(IOutgoingCommand *command)
{
    d->sendOutgoingCommand(command);
}

bool HardwareClient::connectToServer()
{
    return d->connectToServer();
}

void HardwareClient::close()
{
    d->close();
}

bool HardwareClient::isOpen() const
{
    return d->isOpen();
}
