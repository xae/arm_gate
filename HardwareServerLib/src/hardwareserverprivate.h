#ifndef HARDWARESERVERPRIVATE_H
#define HARDWARESERVERPRIVATE_H

#include <QHash>
#include <QObject>
#include <QAtomicInt>
#include <QHostAddress>

#include <tcpasyncserver.h>
#include <abstractasyncclient.h>

#include "iincomingpackethandler.h"
#include "ioutgoingcommand.h"

namespace ArmHardwareServer {

class HardwareServer;

class HardwareServerPrivate : public QObject
{
    Q_OBJECT
public:
    explicit HardwareServerPrivate(HardwareServer *q, const quint16 port, const QHostAddress &address, QObject *parent = 0);
    ~HardwareServerPrivate();

    void registerIncomingCommandHandler(IIncomingPacketHandler *handler);
    void sendOutgoingCommand(IOutgoingCommand *cmd);

    bool startListen();
    void close();

signals:

public slots:
    void onNewConnection();

    void onStateChanged(AsyncClient::AbstractAsyncClient::ClientState state);
    void onIncomingPacket(const AsyncClient::PayloadPacketSharedPtr &packet);

public slots:
    void sendResponse(const int clientId, const QByteArray &packetId, const QByteArray &payload, const quint16 respCode);

private:
    AsyncClient::TcpAsyncServer *mServer;
    friend class HardwareServer;
    HardwareServer *q;
    QHash<quint16, IIncomingPacketHandler*> mCommandHandlers;
    QHash<int, AsyncClient::AbstractAsyncClient*> mClients;
    QAtomicInt mClinetIdGenerator;

};

}
#endif // HARDWARESERVERPRIVATE_H
