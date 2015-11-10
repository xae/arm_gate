#include "hardwarestatuschangeoutgoingcommand.h"

#include <QXmlStreamWriter>

using namespace ArmHardwareServer;

static const char commandTag[] = "command";
static const char idTag[] = "id";

static const char statusTag[] = "status";
static const char statusCheckTimeTag[] = "status_check_time";

static const char okValue[] = "ok";
static const char errorValue[] = "error";

static const char dateTimeFormat[] = "dd.MM.yyyyThh:mm:ss";

ArmHardwareServer::HardwareStatusChangeOutgoingCommand::HardwareStatusChangeOutgoingCommand(
        const QString &hardwareId,
        const HardwareStatusChangeOutgoingCommand::Status &hardwareStatus,
        const QDateTime &statusChangeTime,
        QObject *parent) :
    IOutgoingCommand(parent),
    mHardwareId(hardwareId),
    mHardwareStatus(hardwareStatus),
    mStatusChangeTime(statusChangeTime)

{

}

quint16 ArmHardwareServer::HardwareStatusChangeOutgoingCommand::code() const
{
    return 0x0002;
}

QByteArray ArmHardwareServer::HardwareStatusChangeOutgoingCommand::payload() const
{
    QByteArray payload;
    {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();
        w.writeStartElement(commandTag);

        w.writeTextElement(idTag, mHardwareId);
        w.writeTextElement(statusTag, mHardwareStatus == Ok ?
                               okValue : errorValue);
        w.writeTextElement(statusCheckTimeTag, mStatusChangeTime.toString(dateTimeFormat));

        w.writeEndElement();
        w.writeEndDocument();
    }
    return payload;
}
