#ifndef REQUESTHARDWARESTATUSINCOMINGCOMMANDHANDLER_H
#define REQUESTHARDWARESTATUSINCOMINGCOMMANDHANDLER_H

#include "iincomingpackethandler.h"

namespace ArmHardwareServer {

class RequestHardwareStatusIncomingCommandHandler : public IIncomingPacketHandler
{
    Q_OBJECT
public:
    explicit RequestHardwareStatusIncomingCommandHandler(QObject *parent = 0);

    // IIncomingPacketHandler interface
    virtual void handleIncomingCommand(const int clientId, const AsyncClient::PayloadPacketSharedPtr &packet) override;

signals:

public slots:


    // IIncomingPacketHandler interface
public:
    virtual quint16 commandCode() const override;
};

}

#endif // REQUESTHARDWARESTATUSINCOMINGCOMMANDHANDLER_H
