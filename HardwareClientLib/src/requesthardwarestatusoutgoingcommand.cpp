#include "requesthardwarestatusoutgoingcommand.h"
#include <armhardwareclientcommand.h>

#include <QXmlStreamWriter>
#include <QStringList>

using namespace ArmHardwareClient;

const static char responseTag[] = "response";
const static char idAttribute[] = "id";
const static char hardwareTag[] = "hardware";
const static char statusTag[] = "status";
const static char notifyStatusTag[] = "notify_status";
const static char notifyStatusCheckTimeTag[] = "notify_status_check_time";
const static char statusCheckTimeTag[] = "status_check_time";
const static char okValue[] = "ok";
const static char errorValue[] = "error";
const static char waitValue[] = "wait";
const static char processValue[] = "process";
const static char requestHardwareStatusTag[] = "request_hardware_status";

const static char dateTimeFormat[] = "dd.MM.yyyyThh:mm:ss";

class ArmHardwareClient::RequestHardwareStatusOutgoingCommandPrivate {
public:
    void fillPayload(const QStringList &hardwaresId) {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();

        w.writeStartElement(requestHardwareStatusTag);

        for (const QString &hardwareId : hardwaresId) {
            w.writeTextElement(hardwareTag, hardwareId);
        }

        w.writeEndElement();

        w.writeEndDocument();
    }

    QByteArray payload;
    QList<HardwareStatus> hardwareStatuses;
};

RequestHardwareStatusOutgoingCommand::RequestHardwareStatusOutgoingCommand(const QStringList &hardwaresId, QObject *parent) :
    IOutgoingCommand(parent),
    d(new RequestHardwareStatusOutgoingCommandPrivate())
{
    d->fillPayload(hardwaresId);
}

RequestHardwareStatusOutgoingCommand::~RequestHardwareStatusOutgoingCommand()
{
    delete d;
}

QList<HardwareStatus> RequestHardwareStatusOutgoingCommand::hardwareStatuses() const
{
    return d->hardwareStatuses;
}



IOutgoingCommand::Type ArmHardwareClient::RequestHardwareStatusOutgoingCommand::commandType() const
{
    return RequestHardwareStates;
}

QByteArray ArmHardwareClient::RequestHardwareStatusOutgoingCommand::payload() const
{
    return d->payload;
}

HardwareStatus readHardwareStatus(QXmlStreamReader *reader, bool *ok = nullptr)
{
    HardwareStatus retVal;
    bool isOk = false;
    retVal.hardwareId = reader->attributes().value(idAttribute).toString();
    QString statusText;
    QString notifyStatusText;
    QString notifyStatusCheckTimeText;
    QString statusCheckTimeText;

    while (!reader->atEnd() && !reader->hasError()) {
        switch (reader->readNext()) {
        case QXmlStreamReader::StartElement:
        {
            if (reader->name() == statusTag) {
                statusText = reader->readElementText();
            } else if (reader->name() == notifyStatusTag) {
                notifyStatusText = reader->readElementText();
            } else if (reader->name() == notifyStatusCheckTimeTag) {
                notifyStatusCheckTimeText = reader->readElementText();
            } else if (reader->name() == statusCheckTimeTag) {
                statusCheckTimeText = reader->readElementText();
            }
        }
            break;
        case QXmlStreamReader::EndElement: {
            if (reader->name() == hardwareTag) {
                isOk = true;
            }
        }
            break;

        default:
            break;
        }
        if (isOk) {
            break;
        }
    }
    if (isOk) {
        retVal.notifyStatus = notifyStatusText == processValue ?
                    HardwareStatus::Process : HardwareStatus::Wait;
        retVal.status = statusText == okValue ?
                    HardwareStatus::Ok : HardwareStatus::Error;
        retVal.notifyStatusCheckTime = QDateTime::fromString(notifyStatusCheckTimeText, dateTimeFormat);
        retVal.statusCheckTime = QDateTime::fromString(statusCheckTimeText, dateTimeFormat);
    }

    if (ok) {
        *ok = isOk && !retVal.hardwareId.isEmpty();
    }
    return retVal;
}

void ArmHardwareClient::RequestHardwareStatusOutgoingCommand::doneImpl()
{
    bool result = false;
    ArmHardwareClientCommand *cmd = qobject_cast<ArmHardwareClientCommand*>(sender());
    if (cmd) {
        if (cmd->responsePacket()->command() == 0x0000) {
            QByteArray payload = cmd->responsePacket()->packetPayload();
            QXmlStreamReader r(payload);
            result = true;
            // разбираем пакет согласно спецификации
            while (!r.atEnd() && !r.hasError()) {
                switch (r.readNext()) {
                case QXmlStreamReader::StartElement: {
                    if (r.name() == hardwareTag) {
                        bool ok;
                        HardwareStatus status = readHardwareStatus(&r, &ok);
                        if (ok) {
                            d->hardwareStatuses.append(status);
                        }
                    }
                    break;
                default:
                        break;

                    }
                }
            }
        }
    }
    emit finish(result);
}

void ArmHardwareClient::RequestHardwareStatusOutgoingCommand::errorImpl()
{
    emit finish(false);
}
