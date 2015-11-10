#include "requesthardwarestatusincomingcommandhandler.h"

#include "requesthardwarestatusincomingcommandimpl.h"

#include <QXmlStreamReader>

using namespace ArmHardwareServer;

RequestHardwareStatusIncomingCommandHandler::RequestHardwareStatusIncomingCommandHandler(QObject *parent) :
    IIncomingPacketHandler(parent)
{

}

void ArmHardwareServer::RequestHardwareStatusIncomingCommandHandler::handleIncomingCommand(const int clientId, const AsyncClient::PayloadPacketSharedPtr &packet)
{
    QByteArray payload = packet->packetPayload();
    QXmlStreamReader reader(payload);
    QStringList hardwaresIds;
    while (!reader.atEnd() || !reader.hasError()) {
        switch (reader.readNext()) {
        case QXmlStreamReader::StartElement: {
            QString hardwareId = reader.readElementText();
            if (!hardwareId.isEmpty()) {
                hardwaresIds.append(hardwareId);
            }
        }
            break;
        default:
            break;
        }
    }

    auto cmd = new RequestHardwareStatusIncomingCommandImpl(packet->packetId(), clientId);
    cmd->setRequestedHardwaresIds(hardwaresIds);
#if QT_VERSION >= 0x050000
    connect(cmd, &RequestHardwareStatusIncomingCommandImpl::response,
            this, &RequestHardwareStatusIncomingCommandHandler::response);
#else
    connect(cmd, SIGNAL(response(int,QByteArray,QByteArray,quint16)),
            this, SIGNAL(response(int,QByteArray,QByteArray,quint16)));
#endif

    emit incommingCommand(IIncomingCommandSharedPtr(cmd));

}


quint16 ArmHardwareServer::RequestHardwareStatusIncomingCommandHandler::commandCode() const
{
    return 0x0002;
}
