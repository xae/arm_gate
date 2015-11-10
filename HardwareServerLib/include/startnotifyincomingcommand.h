#ifndef STARTNOTIFYINCOMINGCOMMAND_H
#define STARTNOTIFYINCOMINGCOMMAND_H

#include "hardwareserver_global.h"

#include "iincomingcommand.h"

namespace ArmHardwareServer {

struct NotifyContext
{
    QString notifyId;
    QString description;
    QString notifyCode;
    QString smsText;
    QString synthesisText;
    QString emailSubject;
    QString emailBody;

    QString multimediaFileHash;
    QString multimediaFileName;
    QString multimediaLocalFilePath;

    QString synthesisTextMediaFileHash;
    QString synthesisTextMediaFileName;
    QString synthesisTextMediaLocalFilePath;
};

class HARDWARESERVERSHARED_EXPORT StartNotifyIncomingCommand : public IIncomingCommand
{
    Q_OBJECT
public:
    StartNotifyIncomingCommand(QObject *parent = 0);

    virtual Type commandCode() const override;
    virtual QStringList hardwaresUuids() const = 0;
    virtual NotifyContext notifyContext() const = 0;

    virtual void sendResponse(const QStringList &hardwaresUuids) = 0;
};

}

#endif // STARTNOTIFYINCOMINGCOMMAND_H
