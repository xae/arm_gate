#ifndef REQUESTHARDWARESTATUSINCOMINGCOMMANDIMPL_H
#define REQUESTHARDWARESTATUSINCOMINGCOMMANDIMPL_H

#include "requesthardwarestatusincomingcommand.h"

namespace ArmHardwareServer {

class RequestHardwareStatusIncomingCommandImpl : public RequestHardwareStatusIncomingCommand
{
    Q_OBJECT
public:
    explicit RequestHardwareStatusIncomingCommandImpl(const QByteArray &commandId, const int &clientId,QObject *parent = 0);

    // RequestHardwareStatusIncomingCommand interface
    virtual QStringList requestedHardwaresIds() const override;
    virtual void sendResponse(const QList<HardwareStatus> &statuses) override;

    void setRequestedHardwaresIds(const QStringList &requestedHardwaresIds);

signals:
    void response(const int clientId, const QByteArray &packetId, const QByteArray &payload, const quint16 respCode);

public slots:

private:
    const QByteArray mCommandId;
    const int mClientId;

    QStringList mRequestedHardwaresIds;
};

}

#endif // REQUESTHARDWARESTATUSINCOMINGCOMMANDIMPL_H
