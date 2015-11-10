#ifndef NOTIFYSTATUSCHANGEOUTGOINGCOMMAND_H
#define NOTIFYSTATUSCHANGEOUTGOINGCOMMAND_H

#include "hardwareserver_global.h"

#include <QObject>
#include <QDateTime>

#include "ioutgoingcommand.h"

namespace ArmHardwareServer {

class HARDWARESERVERSHARED_EXPORT NotifyStatusChangeOutgoingCommand : public IOutgoingCommand
{
    Q_OBJECT

public:
    enum Status {
        Wait,
        Process
    };

public:
    explicit NotifyStatusChangeOutgoingCommand(const QString &hardwareId, const Status &notifyStatus, const QDateTime &notifyChangeTime, QObject *parent = 0);

    // IOutgoingCommand interface
public:
    virtual quint16 code() const override;
    virtual QByteArray payload() const override;

private:
    const QString mHardwareId;
    const Status mNotifyStatus;
    const QDateTime mNotifyChangeTime;
};

}
#endif // NOTIFYSTATUSCHANGEOUTGOINGCOMMAND_H
