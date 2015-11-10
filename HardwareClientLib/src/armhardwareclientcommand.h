#ifndef ARMHARDWARECLIENTCOMMAND_H
#define ARMHARDWARECLIENTCOMMAND_H

#include "abstractasyncclient.h"

namespace ArmHardwareClient {

class ArmHardwareClientCommand : public AsyncClient::ICommand
{
    Q_OBJECT
public:
    ArmHardwareClientCommand(AsyncClient::PayloadPacketSharedPtr outgoingPacket, QObject *parent = 0);
    virtual ~ArmHardwareClientCommand() {}
    AsyncClient::PayloadPacketSharedPtr responsePacket() const;


    // ICommand interface
public:
    virtual AsyncClient::PayloadPacketSharedPtr outgoingPacket() const override;
    virtual void done(const AsyncClient::PayloadPacketSharedPtr &responsePacket) override;
    virtual void error(Error errorCode) override;

signals:
    void done();
    void error();

private:
    AsyncClient::PayloadPacketSharedPtr mResponsePacket;
    AsyncClient::PayloadPacketSharedPtr mOutgoingPacket;
};

}
#endif // ARMHARDWARECLIENTCOMMAND_H
