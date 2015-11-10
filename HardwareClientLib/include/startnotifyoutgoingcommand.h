#ifndef STARTNOTIFYOUTGOINGCOMMAND_H
#define STARTNOTIFYOUTGOINGCOMMAND_H

#include <QObject>

#include "hardwareclient_global.h"
#include "ioutgoingcommand.h"

namespace ArmHardwareClient {

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

class StartNotifyOutgoingCommandPrivate;

class HARDWARE_CLIENTSHARED_EXPORT StartNotifyOutgoingCommand : public IOutgoingCommand
{
    Q_OBJECT
public:
    explicit StartNotifyOutgoingCommand(const QStringList &hardwaresIds, const NotifyContext &notifyContext, QObject *parent = 0);
    ~StartNotifyOutgoingCommand();

    QStringList acceptedIds() const;

signals:

public slots:

    // IOutgoingCommand interface
public:
    virtual Type commandType() const override;
    virtual QByteArray payload() const override;

protected:
    virtual void doneImpl() override;
    virtual void errorImpl() override;

private:
    StartNotifyOutgoingCommandPrivate *d;
};

}

#endif // STARTNOTIFYOUTGOINGCOMMAND_H
