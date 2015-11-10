#include "hardwareserver.h"
#include "hardwareserverprivate.h"
#include "outgoingservercommand.h"
#include "startnotifyincomingcommandthandler.h"
#include "requesthardwarestatusincomingcommandhandler.h"

using namespace ArmHardwareServer;

static const char signatureData[] = {0x56, (char)0xa6, 0x59, (char)0x98};
static const QByteArray signature = QByteArray(signatureData, 4);
static const char clientIdProp[] = "clinetId";

HardwareServerPrivate::HardwareServerPrivate(HardwareServer *q, const quint16 port, const QHostAddress &address, QObject *parent) :
    QObject(parent),
    mServer(new AsyncClient::TcpAsyncServer(signature, address, port, this)),
    q(q)
{
#if QT_VERSION >= 0x050000
    connect(mServer, &AsyncClient::TcpAsyncServer::newConnection,
            this, &HardwareServerPrivate::onNewConnection);
#else
    connect(mServer, SIGNAL(newConnection()),
            this, SLOT(onNewConnection()));
#endif


    registerIncomingCommandHandler(new RequestHardwareStatusIncomingCommandHandler(this));
    registerIncomingCommandHandler(new StartNotifyIncomingCommandtHandler(this));
}

HardwareServerPrivate::~HardwareServerPrivate()
{
    mServer->close();
    mServer->deleteLater();
}

void HardwareServerPrivate::registerIncomingCommandHandler(IIncomingPacketHandler *handler)
{
    mCommandHandlers[handler->commandCode()] = handler;
#if QT_VERSION >= 0x050000
    connect(handler, &IIncomingPacketHandler::incommingCommand,
            q, &HardwareServer::incommingCommand);
    connect(handler, &IIncomingPacketHandler::response,
            this, &HardwareServerPrivate::sendResponse);
#else
    connect(handler, SIGNAL(incommingCommand(IIncomingCommandSharedPtr)),
            q, SIGNAL(incommingCommand(IIncomingCommandSharedPtr)));
    connect(handler, SIGNAL(response(int,QByteArray,QByteArray,quint16)),
            this, SLOT(sendResponse(int,QByteArray,QByteArray,quint16)));
#endif
}

void HardwareServerPrivate::sendOutgoingCommand(IOutgoingCommand *cmd)
{
    QHashIterator<int, AsyncClient::AbstractAsyncClient*> it(mClients);
    while (it.hasNext()) {
        it.next();
        using namespace AsyncClient;
        OutgoingServerCommand *outCmd = new OutgoingServerCommand(signature, cmd->code(), cmd->payload());
        it.value()->addCommad(AsyncClient::ICommandSharedPtr(outCmd));
        cmd->deleteLater();
    }

}

bool HardwareServerPrivate::startListen()
{
    return mServer->listen();
}

void HardwareServerPrivate::close()
{
    mServer->close();
}

void HardwareServerPrivate::onNewConnection()
{
    while (mServer->hasPendingConnection()) {
        using namespace AsyncClient;
        AbstractAsyncClient *client = mServer->nextPendingConnection();
        if (client) {
            int clientId = mClinetIdGenerator.fetchAndAddOrdered(1);
            // запоминаем идентификатор клиента в самом клиенте.
            client->setProperty(clientIdProp, clientId);
            mClients[clientId] = client;
#if QT_VERSION >= 0x050000
            connect(client, &AsyncClient::AbstractAsyncClient::incomingPacket,
                    this, &HardwareServerPrivate::onIncomingPacket);
            connect(client, &AsyncClient::AbstractAsyncClient::stateChanged,
                    this, &HardwareServerPrivate::onStateChanged);
#else
            connect(client, SIGNAL(incomingPacket(AsyncClient::PayloadPacketSharedPtr)),
                    this, SLOT(onIncomingPacket(AsyncClient::PayloadPacketSharedPtr)));
            connect(client, SIGNAL(stateChanged(AsyncClient::AbstractAsyncClient::ClientState)),
                    this, SLOT(onStateChanged(AsyncClient::AbstractAsyncClient::ClientState)));
#endif
        }
    }
}

void HardwareServerPrivate::onStateChanged(AsyncClient::AbstractAsyncClient::ClientState state)
{
    using namespace AsyncClient;
    AbstractAsyncClient *client = qobject_cast<AbstractAsyncClient*>(sender());
    if (client) {
        switch (state) {
        case AbstractAsyncClient::DisconnectedState:
        case  AbstractAsyncClient::DisconnectedByServerState: {
            // при потере соединения с клиентом достаем его идентификатор
            int clientId = client->property(clientIdProp).toInt();
            // удаляем его из активных клиентов
            mClients.remove(clientId);
            // и совобождаем память
            client->deleteLater();
        }
            break;
        default:
            break;
        }
    }
}

void HardwareServerPrivate::onIncomingPacket(const AsyncClient::PayloadPacketSharedPtr &packet)
{
    using namespace AsyncClient;
    AbstractAsyncClient *client = qobject_cast<AbstractAsyncClient*>(sender());
    // достаем обработчик пакета в зависимости от команды
    IIncomingPacketHandler *packetHandler = mCommandHandlers.value(packet->command(), nullptr);
    if (packetHandler && client) {
        // идентификатор клиента
        int clinetId = client->property(clientIdProp).toInt();
        // посылаем пакет на обработку
        packetHandler->handleIncomingCommand(clinetId, packet);
    }
}

void HardwareServerPrivate::sendResponse(const int clientId, const QByteArray &packetId, const QByteArray &payload, const quint16 respCode)
{
    using namespace AsyncClient;
    auto client = mClients.value(clientId, nullptr);
    if (client) {
        PayloadPacket *resp = new PayloadPacket(signature);
        resp->setCommand(respCode);
        resp->setPacketPayload(payload);
        resp->setPacketId(packetId);
        client->writeResponse(PayloadPacketSharedPtr(resp));
    }
}

