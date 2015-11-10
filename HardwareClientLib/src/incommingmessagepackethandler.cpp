#include "incommingmessagepackethandler.h"
#include "messageincomingcommandimpl.h"

#include <QXmlStreamReader>

using namespace ArmHardwareClient;

static const char idTag[] = "id";
static const char messageTag[] = "message";

ArmHardwareClient::IIncomingCommand *ArmHardwareClient::IncommingMessagePacketHandler::outgoingCommand(const QByteArray &packetPayload)
{
    IIncomingCommand *retVal = nullptr;
    QXmlStreamReader reader(packetPayload);

    QString hardwareId;
    QString message;
    while (!reader.atEnd() && !reader.hasError()) {
        switch (reader.readNext()) {
        case QXmlStreamReader::StartElement:
            if (reader.name() == idTag) {
                hardwareId = reader.readElementText();
            } else if (reader.name() == messageTag) {
                message = reader.readElementText();
            }
            break;
        default:
            break;
        }
    }

    if (!message.isEmpty() && !hardwareId.isEmpty()) {
        retVal = new MessageIncomingCommandImpl(hardwareId, message);
    }
    return retVal;
}
