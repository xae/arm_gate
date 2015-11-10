#include "startnotifyincomingcommandthandler.h"
#include "startnotifyincomingcommandimpl.h"
#include "startnotifyincomingcommand.h"

#include <QXmlStreamReader>

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

using namespace ArmHardwareServer;

StartNotifyIncomingCommandtHandler::StartNotifyIncomingCommandtHandler(QObject *parent) :
    IIncomingPacketHandler(parent)
{

}

static void readFileInfo(QXmlStreamReader *reader, const QString &closeTag,  QString &fileHash, QString &fileName, QString &filepath) {
    while (!reader->atEnd() || !reader->hasError()) {
        QXmlStreamReader::TokenType token = reader->readNext();
        switch (token) {
        case QXmlStreamReader::StartElement: {
            if (reader->name() == fileHashTag) {
                fileHash = reader->readElementText();
            } else if (reader->name() == fileNameTag) {
                fileName = reader->readElementText();
            } else if (reader->name() == filepathTag) {
                filepath = reader->readElementText();
            }
        }
            break;
        case QXmlStreamReader::EndElement: {
            if (reader->name() == closeTag) {
                return;
            }
        }
            break;

        default:
            break;
        }
    }
}

static QStringList readHardwaresIds(QXmlStreamReader *reader) {
    QStringList hardwares;
    while (!reader->atEnd() || !reader->hasError()) {
        switch (reader->readNext()) {
        case QXmlStreamReader::StartElement: {
            if (reader->name() == uuidTag) {
                QString hardwareId = reader->readElementText();
                if (!hardwareId.isEmpty()) {
                    hardwares.append(hardwareId);
                }
            }
        }
            break;
        case QXmlStreamReader::EndElement:
        {
            if (reader->name() == hardwaresTag) {
                return hardwares;
            }
        }


        default:
            break;
        }
    }
    return hardwares;
}

void StartNotifyIncomingCommandtHandler::handleIncomingCommand(const int clientId, const AsyncClient::PayloadPacketSharedPtr &packet)
{

    StartNotifyIncomingCommandImpl *cmd = new StartNotifyIncomingCommandImpl(packet->packetId(), clientId);

    QByteArray payload = packet->packetPayload();
    QXmlStreamReader reader(payload);
    NotifyContext context;
    QStringList hardwaresId;

    QHash<QString, QString*> values;
    values[descriptionTag] = &context.description;
    values[codeTag] = &context.notifyCode;
    values[smsTextTag] = &context.smsText;
    values[synthesisTextTag] = &context.synthesisText;
    values[emailSubjectTag] = &context.emailSubject;
    values[emailBodyTag] = &context.emailBody;


    while (!reader.atEnd() || !reader.hasError()) {
        QXmlStreamReader::TokenType token = reader.readNext();
        switch (token) {
        case QXmlStreamReader::StartElement:
        {
            QString tag = reader.name().toString();
            if (values.contains(tag)) {
                *values[tag] = reader.readElementText();
            } else {
                if (tag == multimediaFileTag) {
                    readFileInfo(&reader, tag,
                                 context.multimediaFileHash,
                                 context.multimediaFileName,
                                 context.multimediaLocalFilePath);
                } else if (tag == synthesisMediaFileTag) {
                    readFileInfo(&reader, tag,
                                 context.synthesisTextMediaFileHash,
                                 context.synthesisTextMediaFileName,
                                 context.synthesisTextMediaLocalFilePath);
                } else if (tag == hardwaresTag) {
                    hardwaresId = readHardwaresIds(&reader);
                } else if (tag == notifyTag) {
                    context.notifyId = reader.attributes().value(idAttribute).toString();
                }
            }
        }
            break;
        default:
            break;
        }
    }

#if QT_VERSION >= 0x050000
    connect(cmd, &StartNotifyIncomingCommandImpl::response,
            this, &StartNotifyIncomingCommandtHandler::response);
#else
    connect(cmd, SIGNAL(response(int,QByteArray,QByteArray,quint16)),
            this, SIGNAL(response(int,QByteArray,QByteArray,quint16)));
#endif
    cmd->setHardwaresUuids(hardwaresId);
    cmd->setNotifyContext(context);
    emit incommingCommand(IIncomingCommandSharedPtr(cmd));

}





quint16 ArmHardwareServer::StartNotifyIncomingCommandtHandler::commandCode() const
{
    return 0x0001;
}
