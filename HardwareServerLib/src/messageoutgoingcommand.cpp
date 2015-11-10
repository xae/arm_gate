#include "messageoutgoingcommand.h"

#include <QXmlStreamWriter>

using namespace ArmHardwareServer;

static const char commandTag[] = "command";
static const char idTag[] = "id";
static const char messageTag[] = "message";

MessageOutgoingCommand::MessageOutgoingCommand(
        const QString &hardwareId,
        const QString &message,
        QObject *parent) :
    IOutgoingCommand(parent),
    mHardwareId(hardwareId),
    mMessage(message)
{

}

quint16 ArmHardwareServer::MessageOutgoingCommand::code() const
{
    return 0x0003;
}

QByteArray ArmHardwareServer::MessageOutgoingCommand::payload() const
{
    QByteArray payload;
    {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();
        w.writeStartElement(commandTag);

        w.writeTextElement(idTag, mHardwareId);
        w.writeTextElement(messageTag, mMessage);

        w.writeEndElement();
        w.writeEndDocument();
    }
    return payload;
}
