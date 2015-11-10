#ifndef HARDWARESERVER_H
#define HARDWARESERVER_H

#include "hardwareserver_global.h"
#include <QObject>
#include <QHostAddress>

#include "iincomingcommand.h"
#include "ioutgoingcommand.h"

namespace ArmHardwareServer {

class HardwareServerPrivate;

class HARDWARESERVERSHARED_EXPORT HardwareServer : public QObject
{
    Q_OBJECT
public:
    HardwareServer(const quint16 port, const QHostAddress &address, QObject *parent);
    ~HardwareServer();

    bool startListen();
    void close();

    /* время жизни cmd контролируется сервером.
     * команды:
     * HardwareStatusChangeOutgoingCommand
     * MessageOutgoingCommand
     * NotifyStatusChangeOutgoingCommand
     * */
    void sendOutgoingCommand(IOutgoingCommand *cmd);

signals:
    void incommingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command);

private:
    friend class HardwareServerPrivate;
    HardwareServerPrivate *d;
};

}

#endif // HARDWARESERVER_H
