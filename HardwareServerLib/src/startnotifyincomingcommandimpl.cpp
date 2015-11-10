#include "startnotifyincomingcommandimpl.h"
#include <QXmlStreamWriter>

using namespace ArmHardwareServer;

const static char responseTag[] = "response";
const static char uuidTag[] = "uuid";

StartNotifyIncomingCommandImpl::StartNotifyIncomingCommandImpl(const QByteArray &commandId, const int &clientId, QObject *parent) :
    StartNotifyIncomingCommand(parent),
    mCommandId(commandId),
    mClientId(clientId)
{

}

QStringList ArmHardwareServer::StartNotifyIncomingCommandImpl::hardwaresUuids() const
{
    return mHardwaresUuids;
}

NotifyContext ArmHardwareServer::StartNotifyIncomingCommandImpl::notifyContext() const
{
    return mNotifyContext;
}

void ArmHardwareServer::StartNotifyIncomingCommandImpl::sendResponse(const QStringList &hardwaresUuids)
{
    QByteArray payload;
    {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();
        w.writeStartElement(responseTag);
        for (const QString &uuid : hardwaresUuids) {
            w.writeTextElement(uuidTag, uuid);
        }
        w.writeEndElement();
        w.writeEndDocument();
    }
    emit response(mClientId, mCommandId, payload, 0x0000);
}

void StartNotifyIncomingCommandImpl::setNotifyContext(const NotifyContext &notifyContext)
{
    mNotifyContext = notifyContext;
}

void StartNotifyIncomingCommandImpl::setHardwaresUuids(const QStringList &hardwaresUuids)
{
    mHardwaresUuids = hardwaresUuids;
}
