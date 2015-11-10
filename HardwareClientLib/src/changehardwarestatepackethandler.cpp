#include "changehardwarestatepackethandler.h"
#include "changehardwarestatusincomingcommandimpl.h"

#include <QXmlStreamReader>
#include <QDateTime>

static const char commandTag[] = "command";
static const char idTag[] = "id";

static const char statusTag[] = "status";
static const char statusCheckTimeTag[] = "status_check_time";

static const char okValue[] = "ok";
static const char errorValue[] = "error";

static const char dateTimeFormat[] = "dd.MM.yyyyThh:mm:ss";

using namespace ArmHardwareClient;

IIncomingCommand *ChangeHardwareStatePacketHandler::outgoingCommand(const QByteArray &packetPayload)
{
    IIncomingCommand *retVal = nullptr;
    QXmlStreamReader reader(packetPayload);

    QString hardwareId;
    QString textStatus;
    QString testTime;
    while (!reader.atEnd() && !reader.hasError()) {
        switch (reader.readNext()) {
        case QXmlStreamReader::StartElement:
            if (reader.name() == idTag) {
                hardwareId = reader.readElementText();
            } else if (reader.name() == statusTag) {
                textStatus = reader.readElementText();
            } else if (reader.name() == statusCheckTimeTag) {
                testTime = reader.readElementText();
            }
            break;
        default:
            break;
        }
    }

    if (!hardwareId.isEmpty() && !textStatus.isEmpty() && !testTime.isEmpty())  {
        QDateTime statusChangeTime = QDateTime::fromString(testTime, dateTimeFormat);
        ChangeHardwareStatusIncomingCommand::HardwareState state = textStatus == okValue ?
                    ChangeHardwareStatusIncomingCommand::Ok :
                    ChangeHardwareStatusIncomingCommand::Error;
        retVal = new ChangeHardwareStatusIncomingCommandImpl(hardwareId, state, statusChangeTime);
    }

    return retVal;
}
