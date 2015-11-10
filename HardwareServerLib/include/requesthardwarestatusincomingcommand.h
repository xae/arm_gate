#ifndef REQUESTHARDWARESTATUSINCOMINGCOMMAND_H
#define REQUESTHARDWARESTATUSINCOMINGCOMMAND_H

#include "hardwareserver_global.h"

#include <QObject>
#include <QDateTime>
#include <QStringList>
#include "iincomingcommand.h"

namespace ArmHardwareServer {

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

class HARDWARESERVERSHARED_EXPORT RequestHardwareStatusIncomingCommand : public IIncomingCommand
{
    Q_OBJECT
public:
    explicit RequestHardwareStatusIncomingCommand(QObject *parent = 0);

    // IIncomingCommand interface
    virtual Type commandCode() const override;

    virtual QStringList requestedHardwaresIds() const = 0;
    virtual void sendResponse(const QList<HardwareStatus> &statuses) = 0;

public slots:

};

}

#endif // REQUESTHARDWARESTATUSINCOMINGCOMMAND_H
