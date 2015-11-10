#include "armhardwareclientcommand.h"

using namespace ArmHardwareClient;

ArmHardwareClientCommand::ArmHardwareClientCommand(AsyncClient::PayloadPacketSharedPtr outgoingPacket, QObject *parent) :
    AsyncClient::ICommand(parent),
    mOutgoingPacket(outgoingPacket)
{
}

AsyncClient::PayloadPacketSharedPtr ArmHardwareClientCommand::responsePacket() const
{
    return mResponsePacket;
}

AsyncClient::PayloadPacketSharedPtr ArmHardwareClientCommand::outgoingPacket() const
{
    return mOutgoingPacket;
}

void ArmHardwareClientCommand::done(const AsyncClient::PayloadPacketSharedPtr &responsePacket)
{
        mResponsePacket = responsePacket;
        emit done();
}

void ArmHardwareClientCommand::error(AsyncClient::ICommand::Error errorCode)
{
    Q_UNUSED(errorCode);
    emit error();
}
