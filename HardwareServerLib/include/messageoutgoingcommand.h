#ifndef MESSAGEOUTGOINGCOMMAND_H
#define MESSAGEOUTGOINGCOMMAND_H

#include "hardwareserver_global.h"

#include <QObject>

#include "ioutgoingcommand.h"

namespace ArmHardwareServer {

class HARDWARESERVERSHARED_EXPORT MessageOutgoingCommand : public IOutgoingCommand
{
    Q_OBJECT
public:
    explicit MessageOutgoingCommand(const QString &hardwareId, const QString &message, QObject *parent = 0);

signals:

public slots:

    // IOutgoingCommand interface
public:
    virtual quint16 code() const override;
    virtual QByteArray payload() const override;

private:
    const QString mHardwareId;
    const QString mMessage;
};

}

#endif // MESSAGEOUTGOINGCOMMAND_H
