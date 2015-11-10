#include "outgoingservercommand.h"

using namespace ArmHardwareServer;

OutgoingServerCommand::OutgoingServerCommand(const QByteArray &signature, const quint16 commandCode, const QByteArray &payload, QObject *parent) :
    AsyncClient::ICommand(parent),
    mSignature(signature),
    mCommandCode(commandCode),
    mPayload(payload)
{

}



AsyncClient::PayloadPacketSharedPtr ArmHardwareServer::OutgoingServerCommand::outgoingPacket() const
{
    using namespace AsyncClient;
    PayloadPacket *packet = new PayloadPacket(mSignature);
    packet->setCommand(mCommandCode);
    packet->setPacketPayload(mPayload);
    return PayloadPacketSharedPtr(packet);
}

void ArmHardwareServer::OutgoingServerCommand::done(const AsyncClient::PayloadPacketSharedPtr &responsePacket)
{
    // игногрируем отклики
    Q_UNUSED(responsePacket)
}

void ArmHardwareServer::OutgoingServerCommand::error(AsyncClient::ICommand::Error errorCode)
{
    // таже же игнорируем ошибки
    Q_UNUSED(errorCode)
}
