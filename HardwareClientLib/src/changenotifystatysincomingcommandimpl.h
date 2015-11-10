#ifndef CHANGENOTIFYSTATYSINCOMINGCOMMANDIMPL_H
#define CHANGENOTIFYSTATYSINCOMINGCOMMANDIMPL_H

#include <QObject>

#include "changenotifystatysincomingcommand.h"

namespace ArmHardwareClient {

class ChangeNotifyStatysIncomingCommandImpl : public ChangeNotifyStatysIncomingCommand
{
    Q_OBJECT
public:
    explicit ChangeNotifyStatysIncomingCommandImpl(
            const QString &hardwareId,
            const NotifyState &notifyState,
            const QDateTime &stateChangeTime,
            QObject *parent = 0);

signals:

public slots:

    // ChangeNotifyStatysIncomingCommand interface
public:
    virtual QString hardwareId() const override;
    virtual NotifyState notifyState() const override;
    virtual QDateTime stateChangeTime() const override;


private:
    const QString mHardwareId;
    const NotifyState mNotifyState;
    const QDateTime mStateChangeTime;
};

}

#endif // CHANGENOTIFYSTATYSINCOMINGCOMMANDIMPL_H
