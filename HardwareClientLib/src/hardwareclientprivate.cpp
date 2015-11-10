#include "changenotifystatysincomingcommandimpl.h"
#include "changehardwarestatepackethandler.h"
#include "changenotifystatepackethandler.h"
#include "incommingmessagepackethandler.h"
#include "armhardwareclientcommand.h"
#include "iincomingpackethandler.h"
#include "hardwareclientprivate.h"
#include "hardwareclient.h"
#include <tcptransport.h>

using namespace ArmHardwareClient;

static const char signatureData[] = {0x56, (char)0xa6, 0x59, (char)0x98};
static const QByteArray signature = QByteArray(signatureData, 4);

HardwareClientPrivate::HardwareClientPrivate(
        HardwareClient *q,
        const QHostAddress &address,
        const quint16 port, QObject *parent) :
    QObject(parent),
    q(q),
    mClient(new AsyncClient::AbstractAsyncClient(new AsyncClient::TcpTransport(address.toString(), port), signature, this))
{
#if QT_VERSION >= 0x050000
    connect(mClient, &AsyncClient::AbstractAsyncClient::incomingPacket,
            this, &HardwareClientPrivate::onIncomingPacket);
    connect(mClient, &AsyncClient::AbstractAsyncClient::stateChanged,
            this, &HardwareClientPrivate::onStateChanged);
#else
    connect(mClient, SIGNAL(incomingPacket(AsyncClient::PayloadPacketSharedPtr)),
            this, SLOT(onIncomingPacket(AsyncClient::PayloadPacketSharedPtr)));
    connect(mClient, SIGNAL(stateChanged(AsyncClient::AbstractAsyncClient::ClientState)),
            this, SLOT(onStateChanged(AsyncClient::AbstractAsyncClient::ClientState)));
#endif
}

void HardwareClientPrivate::sendOutgoingCommand(IOutgoingCommand *command)
{
    // получаем досуп к приватной имплементации

    if (command) {
        // формируем команду для отправки
        AsyncClient::PayloadPacket *outPacket = new AsyncClient::PayloadPacket(signature);
        outPacket->setPacketPayload(command->payload());
        outPacket->setCommand(static_cast<quint16>(command->commandType()));
        ArmHardwareClientCommand *outCmd = new ArmHardwareClientCommand(AsyncClient::PayloadPacketSharedPtr(outPacket));
#if QT_VERSION >= 0x050000
        connect(outCmd, static_cast<void (ArmHardwareClientCommand::*)()>(&ArmHardwareClientCommand::done), command, &IOutgoingCommand::onDone);
        connect(outCmd, static_cast<void (ArmHardwareClientCommand::*)()>(&ArmHardwareClientCommand::error), command, &IOutgoingCommand::onError);
#else
        connect(outCmd, SIGNAL(done()), command, SLOT(onDone()));
        connect(outCmd, SIGNAL(error()), command, SLOT(onError()));
#endif
        mClient->addCommad(AsyncClient::ICommandSharedPtr(outCmd));
    }
}

bool HardwareClientPrivate::connectToServer()
{
    bool retVal = mClient->open();
    if (!retVal) {
        emit q->error();
    }
    return retVal;
}

void HardwareClientPrivate::close()
{
    return mClient->close();
}

bool HardwareClientPrivate::isOpen() const
{
    return mClient->isOpen();
}

void HardwareClientPrivate::onStateChanged(AsyncClient::AbstractAsyncClient::ClientState state)
{
    using namespace AsyncClient;
    switch (state) {
    case AbstractAsyncClient::ConnectingErrorState:
        emit q->error();
        break;
    case AbstractAsyncClient::ConnectedState:
        emit q->connectedToServer();
        break;
    case AbstractAsyncClient::DisconnectedByServerState:
    case AbstractAsyncClient::DisconnectedState:
        emit q->disconnectedFromServer();
        break;
    default:
        break;
    }
}

void HardwareClientPrivate::onIncomingPacket(const AsyncClient::PayloadPacketSharedPtr &packet)
{
    using namespace AsyncClient;

    // можно было сделать через switch/case, но и так пойдёт :)
    const static QHash<IIncomingCommand::Type, IIncommingPacketHandler*> handlers =
            []() {
        QHash<IIncomingCommand::Type, IIncommingPacketHandler*> retVal;
        retVal[IIncomingCommand::NotifyStatusChange] = new ChangeNotifyStatePacketHandler();
        retVal[IIncomingCommand::HardwareStatusChange] = new ChangeHardwareStatePacketHandler();
        retVal[IIncomingCommand::Message] = new IncommingMessagePacketHandler();
        return retVal;
    }();

    /* на все входящие пакеты отвечаем стандартным
     * пустым откликом */

    auto resp = new PayloadPacket(packet->signature());
    resp->setPacketId(packet->packetId());
    resp->setCommand(0x0000);
    mClient->writeResponse(PayloadPacketSharedPtr(resp));

    // провереям, есть ли обработчик этого пакета
    auto handler = handlers.value(static_cast<IIncomingCommand::Type>(packet->command()), nullptr);
    if (handler) {
        // если есть - отдаём нагрузку пакета этому обработчику
        QByteArray payload = packet->packetPayload();
        auto outCmd = handler->outgoingCommand(payload);
        if (outCmd) {
            // если команда обработана обработчиком - выкидываем ее во внешний мир
            emit q->incomingCommand(IIncomingCommandSharedPtr(outCmd));
        }
    }
}

QString ArmHardwareClient::HardwareClientPrivate::serverAddress() const
{
    QString retVal;
    if (mClient) {
        retVal = mClient->peerAddress();
    }
    return retVal;
}
