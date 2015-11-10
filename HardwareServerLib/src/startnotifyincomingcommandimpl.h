#ifndef STARTNOTIFYINCOMINGCOMMANDIMPL_H
#define STARTNOTIFYINCOMINGCOMMANDIMPL_H

#include "startnotifyincomingcommand.h"

namespace ArmHardwareServer {


class StartNotifyIncomingCommandImpl : public StartNotifyIncomingCommand
{
    Q_OBJECT
public:
    StartNotifyIncomingCommandImpl(const QByteArray &commandId, const int &clientId, QObject *parent = 0);

    void setNotifyContext(const NotifyContext &notifyContext);
    void setHardwaresUuids(const QStringList &hardwaresUuids);

signals:
    void response(const int clientId, const QByteArray &packetId, const QByteArray &payload, const quint16 respCode);

    // StartNotifyIncomingCommand interface
public:
    virtual QStringList hardwaresUuids() const override;
    virtual NotifyContext notifyContext() const override;

    virtual void sendResponse(const QStringList &hardwaresUuids) override;



private:
    const QByteArray mCommandId;
    const int mClientId;
    NotifyContext mNotifyContext;
    QStringList mHardwaresUuids;
};

}

#endif // STARTNOTIFYINCOMINGCOMMANDIMPL_H
