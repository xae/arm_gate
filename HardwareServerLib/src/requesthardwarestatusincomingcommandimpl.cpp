#include "requesthardwarestatusincomingcommandimpl.h"

#include <QXmlStreamWriter>

using namespace ArmHardwareServer;

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

const static char dateTimeFormat[] = "dd.MM.yyyyThh:mm:ss";


RequestHardwareStatusIncomingCommandImpl::RequestHardwareStatusIncomingCommandImpl(const QByteArray &commandId, const int &clientId, QObject *parent) :
    RequestHardwareStatusIncomingCommand(parent),
    mCommandId(commandId),
    mClientId(clientId)
{

}



QStringList ArmHardwareServer::RequestHardwareStatusIncomingCommandImpl::requestedHardwaresIds() const
{
    return mRequestedHardwaresIds;
}

void ArmHardwareServer::RequestHardwareStatusIncomingCommandImpl::sendResponse(const QList<HardwareStatus> &statuses)
{
    QByteArray payload;
    {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();
        w.writeStartElement(responseTag);

        for (const HardwareStatus &status : statuses){
            w.writeStartElement(hardwareTag);
            w.writeAttribute(idAttribute, status.hardwareId);

            w.writeTextElement(statusTag, status.status == HardwareStatus::Ok ?
                                              okValue : errorValue);
            w.writeTextElement(notifyStatusTag, status.notifyStatus == HardwareStatus::Process ?
                                   processValue : waitValue);
            w.writeTextElement(notifyStatusCheckTimeTag, status.notifyStatusCheckTime.toString(dateTimeFormat));
            w.writeTextElement(statusCheckTimeTag, status.statusCheckTime.toString(dateTimeFormat));

            w.writeEndElement();

        }

        w.writeEndElement();
        w.writeEndDocument();
    }

    emit response(mClientId, mCommandId, payload, 0x0000);

}

void RequestHardwareStatusIncomingCommandImpl::setRequestedHardwaresIds(const QStringList &requestedHardwaresIds)
{
    mRequestedHardwaresIds = requestedHardwaresIds;
}
