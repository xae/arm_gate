#ifndef OUTGOINGSERVERCOMMAND_H
#define OUTGOINGSERVERCOMMAND_H

#include <QObject>

#include <icommand.h>

namespace ArmHardwareServer {


class OutgoingServerCommand : public AsyncClient::ICommand
{
    Q_OBJECT
public:
    explicit OutgoingServerCommand(const QByteArray &signature, const quint16 commandCode, const QByteArray &payload, QObject *parent = 0);

signals:

public slots:

    // ICommand interface
public:
    virtual AsyncClient::PayloadPacketSharedPtr outgoingPacket() const override;
    virtual void done(const AsyncClient::PayloadPacketSharedPtr &responsePacket) override;
    virtual void error(Error errorCode) override;

private:
    const QByteArray mSignature;
    const quint16 mCommandCode;
    const QByteArray mPayload;
};

}

#endif // OUTGOINGSERVERCOMMAND_H
