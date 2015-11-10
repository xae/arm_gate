#ifndef HARDWARECLIENT_H
#define HARDWARECLIENT_H

#include "hardwareclient_global.h"

#include <QObject>
#include <QHostAddress>

#include "iincomingcommand.h"
#include "ioutgoingcommand.h"

namespace ArmHardwareClient {

class HardwareClientPrivate;

class HARDWARE_CLIENTSHARED_EXPORT HardwareClient : public QObject
{
    Q_OBJECT

public:
    HardwareClient(const QHostAddress &address, const quint16 port, QObject *parent = 0);
    ~HardwareClient();
    QString serverAddress() const;


    // за время жизни command отвечает вызывающий
    void sendOutgoingCommand(IOutgoingCommand *command);

    bool connectToServer();
    void close();
    bool isOpen() const;

signals:
    void incomingCommand(ArmHardwareClient::IIncomingCommandSharedPtr command);
    void connectedToServer();
    void disconnectedFromServer();
    void error();

private:
    friend class HardwareClientPrivate;
    HardwareClientPrivate *d;
};

}

#endif // HARDWARECLIENT_H
