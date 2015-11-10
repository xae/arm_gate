#include "simplecommand.h"

using namespace AsyncClient;

SimpleCommand::SimpleCommand(const QByteArray &signature, QObject *parent) :
    ICommand(parent),
    mSignature(signature),
    mPacketState(false),
    mLastError(NoError)
{

}

SimpleCommand::~SimpleCommand()
{

}

AsyncClient::PayloadPacketSharedPtr SimpleCommand::outgoingPacket() const
{
    PayloadPacketSharedPtr packet = PayloadPacketSharedPtr(new PayloadPacket(mSignature));
    packet->setPacketPayload(mPayload);
    packet->setCommand(mCommand);
    mPacketId = packet->packetId();
    return packet;
}

void SimpleCommand::done(const AsyncClient::PayloadPacketSharedPtr &responsePacket)
{
    if (responsePacket && responsePacket->packetPayload() == mPayload) {
        mPacketState = true;
    }
}

void SimpleCommand::error(AsyncClient::ICommand::Error error)
{
    mLastError = error;
}

quint16 SimpleCommand::command() const
{
    return mCommand;
}

void SimpleCommand::setCommand(const quint16 &command)
{
    mCommand = command;
}
QByteArray SimpleCommand::payload() const
{
    return mPayload;
}

void SimpleCommand::setPayload(const QByteArray &payload)
{
    mPayload = payload;
}

QByteArray SimpleCommand::packetId() const
{
    return mPacketId;
}

bool SimpleCommand::packetState() const
{
    return mPacketState;
}

void SimpleCommand::setPacketState(bool packetState)
{
    mPacketState = packetState;
}
AsyncClient::ICommand::Error SimpleCommand::lastError() const
{
    return mLastError;
}

void SimpleCommand::setLastError(const AsyncClient::ICommand::Error &lastError)
{
    mLastError = lastError;
}

