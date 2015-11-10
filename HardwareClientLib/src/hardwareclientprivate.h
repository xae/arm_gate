#ifndef HARDWARECLIENTPRIVATE_H
#define HARDWARECLIENTPRIVATE_H

#include <QObject>
#include <QHostAddress>

#include "abstractasyncclient.h"
#include "ioutgoingcommand.h"
namespace ArmHardwareClient {

class HardwareClient;

class HardwareClientPrivate : public QObject
{
    Q_OBJECT
public:
    explicit HardwareClientPrivate(HardwareClient *q, const QHostAddress &address, const quint16 port, QObject *parent = 0);
    void sendOutgoingCommand(IOutgoingCommand *command);
    QString serverAddress() const;

    bool connectToServer();
    void close();
    bool isOpen() const;

signals:

public slots:
    void onStateChanged(AsyncClient::AbstractAsyncClient::ClientState state);
    void onIncomingPacket(const AsyncClient::PayloadPacketSharedPtr &packet);

private:
    friend class HardwareClient;
    HardwareClient *q;
    AsyncClient::AbstractAsyncClient *mClient;
};

}

#endif // HARDWARECLIENTPRIVATE_H
