#ifndef STARTNOTIFYINCOMINGCOMMANDTHANDLER_H
#define STARTNOTIFYINCOMINGCOMMANDTHANDLER_H

#include <QObject>
#include "iincomingpackethandler.h"

namespace ArmHardwareServer {

class StartNotifyIncomingCommandtHandler : public IIncomingPacketHandler
{
    Q_OBJECT
public:
    explicit StartNotifyIncomingCommandtHandler(QObject *parent = 0);

    // IIncomingPacketHandler interface
    virtual void handleIncomingCommand(const int clientId, const AsyncClient::PayloadPacketSharedPtr &packet) override;

public slots:



    // IIncomingPacketHandler interface
public:
    virtual quint16 commandCode() const override;
};

}

#endif // STARTNOTIFYINCOMINGCOMMANDTHANDLER_H
