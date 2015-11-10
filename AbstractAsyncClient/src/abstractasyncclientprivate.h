#ifndef ABSTRACTASYNCCLIENTPRIVATE_H
#define ABSTRACTASYNCCLIENTPRIVATE_H

#include "iabstracttransport.h"
#include "icommand.h"
#include "abstractasyncclient.h"

#include <QObject>
#include <QHash>
#include <QQueue>
#include <QTimer>
#include <QDateTime>

namespace AsyncClient {

class AbstractAsyncClientPrivate : public QObject
{
    Q_OBJECT

    struct CommandInfo {
        ICommandSharedPtr command;
        QDateTime sendTime;
    };

public:
    explicit AbstractAsyncClientPrivate(IAbstractTransport *transport,
                                        const QByteArray &signature,
                                        AbstractAsyncClient *parent = 0);
    ~AbstractAsyncClientPrivate();

private:
    void addCommad(const ICommandSharedPtr &cmd);
    void writeResponse(const PayloadPacketSharedPtr &response);
    void close();
    void closeHelper();
    bool isOpen() const;
    AbstractAsyncClient::ClientState clientState() const;
    QByteArray signature() const;

    void handleIncommingData();
    void handleErrorPacket();
    void handleIncompetePacket();
    void handleCompletePacket(const PayloadPacketSharedPtr &packet);
    void checkQueues();
    void changeState(AbstractAsyncClient::ClientState state);

    void handleDisconnectedTransportState();
    void handleConnectedTransportState();
    void handleConnectingTransportState();
    void handleConnectionErrorTransportState();
    void handleTransportErrorState();
    void handleDisconnectByUserTransportState();

    void markAllWaitingCommandsAsUnprocessed();
    bool sheduleReconnect();
    void startDataTimeoutTimer();
    void stopDataTimeoutTimer();
    void resetReconnectTime();

private slots:
    bool open();
    void onReadyRead();
    void onTransportStateChange(const IAbstractTransport::TransportState state);
    void onTimeout();
    void onCheckTimeoutedCommnads();

private:
    friend class AbstractAsyncClient;
    AbstractAsyncClient *q;
    IAbstractTransport *mTransport;
    QHash<QByteArray, CommandInfo> mWaitingResponseCommands;
    QQueue<ICommandSharedPtr> mCommandsQueue;
    QQueue<PayloadPacketSharedPtr> mResponsesQueue;
    QByteArray mBuffer;
    const QByteArray mSignature;
    AbstractAsyncClient::ClientState mCurrentState;
    QTimer mTimer;
    QTimer mReconnectionTimer;
    QTimer mTimerForCheckTimeoutedCommnads;
    int mErrorCount;
    const int mMaxTimeToWaitResponse;
    const int mMaxErrorCount;
    const int mMinMsToReconnect;
    const int mMaxMsToReconnect;
    int mNextMsToReconnect;
    bool mIsReconnectEnabled;
};

}


#endif // ABSTRACTASYNCCLIENTPRIVATE_H

