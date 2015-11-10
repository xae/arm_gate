#include "abstractasyncclientprivate.h"

using namespace AsyncClient;

AbstractAsyncClientPrivate::AbstractAsyncClientPrivate(IAbstractTransport *transport, const QByteArray &signature,
        AbstractAsyncClient *parent) :
    QObject(parent),
    q(parent),
    mTransport(transport),
    mSignature(signature),
    mCurrentState(AbstractAsyncClient::StoppedState),
    mTimer(this),
    mReconnectionTimer(this),
    mTimerForCheckTimeoutedCommnads(this),
    mErrorCount(0),
    mMaxTimeToWaitResponse(30000),
    mMaxErrorCount(20),
    mMinMsToReconnect(1000),
    mMaxMsToReconnect(120000),
    mNextMsToReconnect(mMinMsToReconnect)
{
    if (!transport->parent()){
        transport->setParent(this);
    }
    connect(transport, SIGNAL(stateChanged(IAbstractTransport::TransportState)),
            this, SLOT(onTransportStateChange(IAbstractTransport::TransportState)));
    connect(transport, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(&mReconnectionTimer, SIGNAL(timeout()), SLOT(open()));
    connect(&mTimerForCheckTimeoutedCommnads, SIGNAL(timeout()), SLOT(onCheckTimeoutedCommnads()));
    mTimer.setSingleShot(true);
    mReconnectionTimer.setSingleShot(true);
    mTimerForCheckTimeoutedCommnads.setSingleShot(false);
}

AbstractAsyncClientPrivate::~AbstractAsyncClientPrivate()
{

}

void AbstractAsyncClientPrivate::addCommad(const ICommandSharedPtr &cmd)
{
    mCommandsQueue.append(cmd);
    checkQueues();
}

void AbstractAsyncClientPrivate::writeResponse(const PayloadPacketSharedPtr &response)
{
    mResponsesQueue.append(response);
    checkQueues();
}

bool AbstractAsyncClientPrivate::open()
{
    mIsReconnectEnabled = true;
    if (!mTransport->isOpen()) {
        changeState(AbstractAsyncClient::ConnectingState);
    }
    return mTransport->open();
}

void AbstractAsyncClientPrivate::close()
{
    mIsReconnectEnabled = false;
    if (mTransport->isOpen()) {
        changeState(AbstractAsyncClient::DisconnectingState);
    } else {
        changeState(AbstractAsyncClient::StoppedState);
    }
    closeHelper();

}

void AbstractAsyncClientPrivate::closeHelper()
{
    mReconnectionTimer.stop();
    mTransport->close();
}

bool AbstractAsyncClientPrivate::isOpen() const
{
    return mTransport->isOpen();
}

void AbstractAsyncClientPrivate::handleIncommingData()
{
    PayloadPacketSharedPtr packet(PayloadPacket::packetFromRawData(mBuffer, mSignature));
    switch (packet->packetState()) {
    case PayloadPacket::CrcError:
    case PayloadPacket::SignatureError:
        handleErrorPacket();
        break;
    case PayloadPacket::Incomplete:
        handleIncompetePacket();
        break;
    case PayloadPacket::Ok:
        handleCompletePacket(packet);
        break;
    }
}

void AbstractAsyncClientPrivate::handleErrorPacket()
{
    mBuffer.clear();
    ++mErrorCount;
    if (mErrorCount >= mMaxErrorCount) {
        mErrorCount = 0;
        changeState(AbstractAsyncClient::DisconnectingDueToErrorState);
        closeHelper();
    }
}

void AbstractAsyncClientPrivate::handleIncompetePacket()
{
    startDataTimeoutTimer();
}

void AbstractAsyncClientPrivate::handleCompletePacket(const PayloadPacketSharedPtr &packet)
{
    mErrorCount = 0;
    /* команды 0x0000, 0xffff, 0xfffe - отклик. */
    switch (packet->command()) {
    case 0x0000:
    case 0xfffe:
    case 0xffff: {
        QByteArray packetId = packet->packetId();
        CommandInfo ci = mWaitingResponseCommands.value(packetId, CommandInfo());
        if (ci.command) {
            ci.command->done(packet);
            mWaitingResponseCommands.remove(packetId);
        }
    }
        break;
    default:
        /* разбор остальных пакетов за пределами ответственности библиотеки */
        emit q->incomingPacket(packet);
        break;
    }
    mBuffer.remove(0, packet->lenght());
    if (mBuffer.length() > 0) {
        handleIncommingData();
    }
}

void AbstractAsyncClientPrivate::checkQueues()
{
    if (!isOpen()) {
        for (const ICommandSharedPtr &cmd : mCommandsQueue) {
            cmd->error(ICommand::NetworkError);
        }
        mCommandsQueue.clear();
        return;
    }
    /* отправляем все ждущие своей очереди комманды */
    QDateTime currentTime(QDateTime::currentDateTime());
    bool hasCommands = false;
    for (const ICommandSharedPtr &cmd : mCommandsQueue) {
        hasCommands = true;
        auto outgoingPacket = cmd->outgoingPacket();
        outgoingPacket->writeBinaryPacket(mTransport);
        CommandInfo ci;
        ci.sendTime = currentTime;
        ci.command = cmd;
        mWaitingResponseCommands[outgoingPacket->packetId()] = ci;
    }
    mCommandsQueue.clear();

    /* отправляем все ждущие своей очереди отклики на вход */
    for (const PayloadPacketSharedPtr &packet : mResponsesQueue) {
        packet->writeBinaryPacket(mTransport);
    }
    mResponsesQueue.clear();
    if (hasCommands && !mTimerForCheckTimeoutedCommnads.isActive()) {
        mTimerForCheckTimeoutedCommnads.start(mMaxTimeToWaitResponse);
    } else if (!hasCommands){
        mTimerForCheckTimeoutedCommnads.stop();
    }
}

void AbstractAsyncClientPrivate::changeState(AbstractAsyncClient::ClientState state)
{
    if (mCurrentState != state) {
        mCurrentState = state;
        emit q->stateChanged(state);
    }
}

void AbstractAsyncClientPrivate::handleDisconnectedTransportState()
{
    markAllWaitingCommandsAsUnprocessed();
    bool needToReconnect = mCurrentState != AbstractAsyncClient::DisconnectingByUserDecision;
    changeState(AbstractAsyncClient::DisconnectedState);
    if (needToReconnect) {
        sheduleReconnect();
        changeState(AbstractAsyncClient::WaitState);
    } else {
        changeState(AbstractAsyncClient::StoppedState);
    }
}

void AbstractAsyncClientPrivate::handleConnectedTransportState()
{
    resetReconnectTime();
    changeState(AbstractAsyncClient::ConnectedState);
}

void AbstractAsyncClientPrivate::handleConnectingTransportState()
{
    changeState(AbstractAsyncClient::ConnectingState);
}

void AbstractAsyncClientPrivate::handleConnectionErrorTransportState()
{
    changeState(AbstractAsyncClient::ConnectingErrorState);
    sheduleReconnect();
}

void AbstractAsyncClientPrivate::handleTransportErrorState()
{
    markAllWaitingCommandsAsUnprocessed();
    changeState(AbstractAsyncClient::DisconnectedState);
    sheduleReconnect();
}

void AbstractAsyncClientPrivate::handleDisconnectByUserTransportState()
{
    changeState(AbstractAsyncClient::DisconnectingByUserDecision);
}

void AbstractAsyncClientPrivate::markAllWaitingCommandsAsUnprocessed()
{
    for (const CommandInfo &ci : mWaitingResponseCommands.values()) {
        if (ci.command) {
            ci.command->error(ICommand::NetworkError);
        }
    }
    mWaitingResponseCommands.clear();
}

bool AbstractAsyncClientPrivate::sheduleReconnect()
{
    if (mIsReconnectEnabled){
        mReconnectionTimer.start(mNextMsToReconnect);
        mNextMsToReconnect *= 1.7f;
        if (mNextMsToReconnect > mMaxMsToReconnect) {
            mNextMsToReconnect = mMaxMsToReconnect;
        }
        changeState(AbstractAsyncClient::WaitState);
    } else {
        changeState(AbstractAsyncClient::StoppedState);
    }
    return mIsReconnectEnabled;
}

void AbstractAsyncClientPrivate::startDataTimeoutTimer()
{
    mTimer.start(mTransport->timeoutWaitDataValue());
}

void AbstractAsyncClientPrivate::stopDataTimeoutTimer()
{
    mTimer.stop();
}

void AbstractAsyncClientPrivate::resetReconnectTime()
{
    mNextMsToReconnect = mMinMsToReconnect;
}

AbstractAsyncClient::ClientState AbstractAsyncClientPrivate::clientState() const
{
    return mCurrentState;
}

QByteArray AbstractAsyncClientPrivate::signature() const
{
    return mSignature;
}

void AbstractAsyncClientPrivate::onReadyRead()
{
    stopDataTimeoutTimer();
    mBuffer.append(mTransport->readAll());
    handleIncommingData();
}

void AbstractAsyncClientPrivate::onTransportStateChange(const IAbstractTransport::TransportState state)
{
    switch (state) {
    case IAbstractTransport::Disconnected:
        handleDisconnectedTransportState();
        break;
    case IAbstractTransport::Connected:
        handleConnectedTransportState();
        break;
    case IAbstractTransport::Connecting:
        handleConnectingTransportState();
        break;
    case IAbstractTransport::ConnectionError:
        handleConnectionErrorTransportState();
        break;
    case IAbstractTransport::TransportError:
        handleTransportErrorState();
        break;
    case IAbstractTransport::DisconnectByUser:
        handleDisconnectByUserTransportState();
        break;
    }
}

void AbstractAsyncClientPrivate::onTimeout()
{
    changeState(AbstractAsyncClient::DisconnectingDueToTimeoutState);
    closeHelper();
}

void AbstractAsyncClientPrivate::onCheckTimeoutedCommnads()
{
    QDateTime currentTime(QDateTime::currentDateTime());
    for (const QByteArray commandId : mWaitingResponseCommands.keys()) {
        CommandInfo ci = mWaitingResponseCommands.value(commandId);
        if (ci.sendTime.msecsTo(currentTime) > mMaxTimeToWaitResponse) {
            if (ci.command) {
                ci.command->error(ICommand::TimeoutWaitResponseError);
            }
            mWaitingResponseCommands.remove(commandId);
        }
    }

    if (mWaitingResponseCommands.count() == 0) {
        mTimerForCheckTimeoutedCommnads.stop();
    }
}
