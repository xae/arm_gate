#ifndef REQUESTHARDWARESTATUSOUTGOINGCOMMAND_H
#define REQUESTHARDWARESTATUSOUTGOINGCOMMAND_H

#include <QObject>
#include <QDateTime>

#include "hardwareclient_global.h"
#include "ioutgoingcommand.h"

namespace ArmHardwareClient {

struct HardwareStatus {
    enum Status {
        Ok,
        Error
    };

    enum NotifyStatus {
        Wait,
        Process
    };

    QString hardwareId;
    Status status;
    QDateTime statusCheckTime;
    NotifyStatus notifyStatus;
    QDateTime notifyStatusCheckTime;
};

class RequestHardwareStatusOutgoingCommandPrivate;

class HARDWARE_CLIENTSHARED_EXPORT RequestHardwareStatusOutgoingCommand : public IOutgoingCommand
{
    Q_OBJECT
public:
    explicit RequestHardwareStatusOutgoingCommand(const QStringList &hardwaresId, QObject *parent = 0);
    ~RequestHardwareStatusOutgoingCommand();
    QList<HardwareStatus> hardwareStatuses() const;

signals:

public slots:

private:
    RequestHardwareStatusOutgoingCommandPrivate *d;

    // IOutgoingCommand interface
public:
    virtual Type commandType() const override;
    virtual QByteArray payload() const override;

protected:
    virtual void doneImpl() override;
    virtual void errorImpl() override;
};

}

#endif // REQUESTHARDWARESTATUSOUTGOINGCOMMAND_H
