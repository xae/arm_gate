#include "changenotifystatepackethandler.h"
#include "changenotifystatysincomingcommandimpl.h"

#include <QXmlStreamReader>
#include <QDateTime>

static const char commandTag[] = "command";
static const char idTag[] = "id";
static const char notifyStatusTag[] = "notify_status";
static const char notifyStatusChangeTimeTag[] = "notify_status_change_time";

static const char waitValue[] = "wait";
static const char processValue[] = "process";

static const char dateTimeFormat[] = "dd.MM.yyyyThh:mm:ss";

using namespace ArmHardwareClient;

ArmHardwareClient::IIncomingCommand *ArmHardwareClient::ChangeNotifyStatePacketHandler::outgoingCommand(const QByteArray &packetPayload)
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
            } else if (reader.name() == notifyStatusTag) {
                textStatus = reader.readElementText();
            } else if (reader.name() == notifyStatusChangeTimeTag) {
                testTime = reader.readElementText();
            }
            break;
        default:
            break;
        }
    }

    if (!hardwareId.isEmpty() && !textStatus.isEmpty() && !testTime.isEmpty())  {
        QDateTime statusChangeTime = QDateTime::fromString(testTime, dateTimeFormat);
        ChangeNotifyStatysIncomingCommand::NotifyState state = textStatus == processValue ?
                    ChangeNotifyStatysIncomingCommand::Process :
                    ChangeNotifyStatysIncomingCommand::Wait;
        retVal = new ChangeNotifyStatysIncomingCommandImpl(hardwareId, state, statusChangeTime);
    }

    return retVal;
}
