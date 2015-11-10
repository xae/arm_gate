#include "notifystatuschangeoutgoingcommand.h"

#include <QXmlStreamWriter>

using namespace ArmHardwareServer;

static const char commandTag[] = "command";
static const char idTag[] = "id";
static const char notifyStatusTag[] = "notify_status";
static const char notifyStatusChangeTimeTag[] = "notify_status_change_time";

static const char waitValue[] = "wait";
static const char processValue[] = "process";

static const char dateTimeFormat[] = "dd.MM.yyyyThh:mm:ss";


NotifyStatusChangeOutgoingCommand::NotifyStatusChangeOutgoingCommand(const QString &hardwareId, const Status &notifyStatus, const QDateTime &notifyChangeTime, QObject *parent) :
    IOutgoingCommand(parent),
    mHardwareId(hardwareId),
    mNotifyStatus(notifyStatus),
    mNotifyChangeTime(notifyChangeTime)
{

}

quint16 ArmHardwareServer::NotifyStatusChangeOutgoingCommand::code() const
{
    return 0x0001;
}

QByteArray ArmHardwareServer::NotifyStatusChangeOutgoingCommand::payload() const
{
    QByteArray payload;
    {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();
        w.writeStartElement(commandTag);

        w.writeTextElement(idTag, mHardwareId);
        w.writeTextElement(notifyStatusTag, mNotifyStatus == Process ?
                               processValue : waitValue);
        w.writeTextElement(notifyStatusChangeTimeTag, mNotifyChangeTime.toString(dateTimeFormat));

        w.writeEndElement();
        w.writeEndDocument();
    }
    return payload;
}
