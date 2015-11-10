#ifndef HARDWARESTATUSCHANGEOUTGOINGCOMMAND_H
#define HARDWARESTATUSCHANGEOUTGOINGCOMMAND_H

#include "hardwareserver_global.h"

#include <QObject>
#include <QDateTime>

#include "ioutgoingcommand.h"

namespace ArmHardwareServer {

class HARDWARESERVERSHARED_EXPORT HardwareStatusChangeOutgoingCommand : public IOutgoingCommand
{
    Q_OBJECT

public:
    enum Status {
        Ok,
        Error
    };

public:
    explicit HardwareStatusChangeOutgoingCommand(
            const QString &hardwareId,
            const Status &hardwareStatus,
            const QDateTime &statusChangeTime,
            QObject *parent = 0);

signals:

public slots:

    // IOutgoingCommand interface
public:
    virtual quint16 code() const override;
    virtual QByteArray payload() const override;

private:
    const QString mHardwareId;
    const Status mHardwareStatus;
    const QDateTime mStatusChangeTime;
};

}

#endif // HARDWARESTATUSCHANGEOUTGOINGCOMMAND_H
