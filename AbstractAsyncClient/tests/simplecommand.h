#ifndef SIMPLECOMMAND_H
#define SIMPLECOMMAND_H
#include "icommand.h"

class SimpleCommand : public AsyncClient::ICommand
{
public:
    SimpleCommand(const QByteArray &signature, QObject *parent = 0);
    ~SimpleCommand();

    quint16 command() const;
    void setCommand(const quint16 &command);

    QByteArray payload() const;
    void setPayload(const QByteArray &payload);

    QByteArray packetId() const;

    // ICommand interface
public:
    virtual AsyncClient::PayloadPacketSharedPtr outgoingPacket() const;
    virtual void done(const AsyncClient::PayloadPacketSharedPtr &responsePacket);
    virtual void error(Error error);

    bool packetState() const;
    void setPacketState(bool packetState);

    AsyncClient::ICommand::Error lastError() const;
    void setLastError(const AsyncClient::ICommand::Error &lastError);

private:
    quint16 mCommand;
    QByteArray mPayload;
    mutable QByteArray mPacketId;
    const QByteArray mSignature;
    bool mPacketState;
    AsyncClient::ICommand::Error mLastError;
};

#endif // SIMPLECOMMAND_H

