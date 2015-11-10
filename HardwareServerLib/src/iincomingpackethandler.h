#ifndef IINCOMINGPACKETHANDLER_H
#define IINCOMINGPACKETHANDLER_H

#include <QObject>
#include "iincomingcommand.h"
#include <abstractasyncclient.h>

namespace ArmHardwareServer {

class IIncomingPacketHandler : public QObject
{
    Q_OBJECT
public:
    IIncomingPacketHandler(QObject *parent = 0) : QObject(parent) {}
    virtual void handleIncomingCommand(const int clientId, const AsyncClient::PayloadPacketSharedPtr &packet) = 0;
    virtual quint16 commandCode() const = 0;

signals:
    void response(const int clientId, const QByteArray &packetId, const QByteArray &payload, const quint16 respCode);
    void incommingCommand(IIncomingCommandSharedPtr command);
};

}
#endif // IINCOMINGPACKETHANDLER_H
