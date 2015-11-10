#include "startnotifyoutgoingcommand.h"
#include "armhardwareclientcommand.h"

#include <QXmlStreamWriter>
#include <QStringList>

static const char notifyTag[] = "notify";
static const char idAttribute[] = "id";
static const char descriptionTag[] = "description";
static const char codeTag[] = "code";
static const char smsTextTag[] = "sms_text";
static const char synthesisTextTag[] = "synthesis_text";
static const char emailSubjectTag[] = "email_subject";
static const char emailBodyTag[] = "email_body";
static const char multimediaFileTag[] = "multimedia_file";
static const char synthesisMediaFileTag[] = "synthesis_media_file";
static const char hardwaresTag[] = "hardwares";
static const char uuidTag[] = "uuid";
static const char fileHashTag[] = "file_hash";
static const char fileNameTag[] = "file_name";
static const char filepathTag[] = "filepath";

using namespace ArmHardwareClient;

class ArmHardwareClient::StartNotifyOutgoingCommandPrivate {
public:
    void fillPayload(const QStringList &hardwaresIds,
                     const NotifyContext &notifyContext) {
        QXmlStreamWriter w(&payload);
        w.writeStartDocument();

        w.writeStartElement(notifyTag);
        w.writeAttribute(idAttribute, notifyContext.notifyId);

        w.writeTextElement(descriptionTag, notifyContext.description);
        w.writeTextElement(codeTag, notifyContext.notifyCode);
        w.writeTextElement(smsTextTag, notifyContext.smsText);
        w.writeTextElement(synthesisTextTag, notifyContext.synthesisText);
        w.writeTextElement(emailBodyTag, notifyContext.emailBody);
        w.writeTextElement(emailSubjectTag, notifyContext.emailSubject);

        w.writeStartElement(multimediaFileTag);
        w.writeTextElement(fileHashTag, notifyContext.multimediaFileHash);
        w.writeTextElement(fileNameTag, notifyContext.multimediaFileName);
        w.writeTextElement(filepathTag, notifyContext.multimediaLocalFilePath);
        w.writeEndElement();

        w.writeStartElement(synthesisMediaFileTag);
        w.writeTextElement(fileHashTag, notifyContext.synthesisTextMediaFileHash);
        w.writeTextElement(fileNameTag, notifyContext.synthesisTextMediaFileName);
        w.writeTextElement(filepathTag, notifyContext.synthesisTextMediaLocalFilePath);
        w.writeEndElement();

        w.writeStartElement(hardwaresTag);
        for (const QString &hardwareId : hardwaresIds) {
            w.writeTextElement(uuidTag, hardwareId);
        }
        w.writeEndElement();


        w.writeEndElement();

        w.writeEndDocument();


    }

    QByteArray payload;
    QStringList acceptedIds;
};

StartNotifyOutgoingCommand::StartNotifyOutgoingCommand(
        const QStringList &hardwaresIds,
        const NotifyContext &notifyContext,
        QObject *parent) :
    IOutgoingCommand(parent),
    d(new StartNotifyOutgoingCommandPrivate())
{
    d->fillPayload(hardwaresIds, notifyContext);
}

StartNotifyOutgoingCommand::~StartNotifyOutgoingCommand()
{
    delete d;
}

QStringList StartNotifyOutgoingCommand::acceptedIds() const
{
    return d->acceptedIds;
}



ArmHardwareClient::IOutgoingCommand::Type ArmHardwareClient::StartNotifyOutgoingCommand::commandType() const
{
    return StartNotify;
}

void ArmHardwareClient::StartNotifyOutgoingCommand::doneImpl()
{
    ArmHardwareClientCommand *cmd = qobject_cast<ArmHardwareClientCommand*>(sender());
    bool success = false;
    if (cmd) {

        if (cmd->responsePacket()->command() == 0x0000) {
            QByteArray payload = cmd->responsePacket()->packetPayload();
            QXmlStreamReader r(payload);
            success = true;
            // разбираем пакет согласно спецификации
            while (!r.atEnd() && !r.hasError()) {
                switch (r.readNext()) {
                case QXmlStreamReader::StartElement: {
                    if (r.name() == uuidTag) {
                        QString uuid = r.readElementText();
                        if (!uuid.isEmpty()) {
                            d->acceptedIds.append(uuid);
                        }
                    }
                }
                    break;
                default:
                    break;
                }
            }
        }
    }
    emit finish(success);
}

void ArmHardwareClient::StartNotifyOutgoingCommand::errorImpl()
{
    emit finish(false);
}


QByteArray ArmHardwareClient::StartNotifyOutgoingCommand::payload() const
{
    return d->payload;
}
