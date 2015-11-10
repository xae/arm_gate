#ifndef ABSTRACTASYNCCLIENT_H
#define ABSTRACTASYNCCLIENT_H

#include "abstractasyncclient_global.h"
#include "icommand.h"
#include "iabstracttransport.h"

#include <QObject>

namespace AsyncClient {

class AbstractAsyncClientPrivate;

class ABSTRACTASYNCCLIENTSHARED_EXPORT AbstractAsyncClient : public QObject
{
    Q_OBJECT
public:
    enum ClientState {
        WaitState,
        ConnectingState,
        ConnectingErrorState,
        DisconnectingState,
        ConnectedState,
        DisconnectingDueToErrorState,
        DisconnectedByServerState,
        DisconnectingDueToTimeoutState,
        DisconnectedState,
        DisconnectingByUserDecision,
        StoppedState
    };

public:
    // если у транспорта не назначен родитель, то за время жизни
    // транспорта отвечает AbstractAsyncClient
    AbstractAsyncClient(IAbstractTransport *transport,
                        const QByteArray &signature,
                        QObject *parent = 0);
    ~AbstractAsyncClient();

    void addCommad(const ICommandSharedPtr &cmd);
    void writeResponse(const PayloadPacketSharedPtr &response);
    bool open();
    void close();
    bool isOpen() const;
    QByteArray signature() const;
    QString peerAddress() const;
    AbstractAsyncClient::ClientState clientState() const;

signals:
    void stateChanged(AsyncClient::AbstractAsyncClient::ClientState state);
    void message(const QString &message);
    void incomingPacket(const AsyncClient::PayloadPacketSharedPtr &packet);

private:
    AbstractAsyncClientPrivate *d;
    friend class AbstractAsyncClientPrivate;
};

}

#endif // ABSTRACTASYNCCLIENT_H
