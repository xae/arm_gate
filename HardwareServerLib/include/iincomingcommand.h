#ifndef IINCOMINGCOMMAND_H
#define IINCOMINGCOMMAND_H

#include "hardwareserver_global.h"
#include <QObject>
#include <QSharedPointer>

namespace ArmHardwareServer {

class HARDWARESERVERSHARED_EXPORT IIncomingCommand : public QObject
{
    Q_OBJECT
public:
    enum Type {
        StartNotify = 0x0001,
        RequestStatuses = 0x0002
    };

public:
    explicit IIncomingCommand(QObject *parent = 0) : QObject(parent) {}
    virtual Type commandCode() const = 0;
};

typedef QSharedPointer<IIncomingCommand> IIncomingCommandSharedPtr;

}

Q_DECLARE_METATYPE(ArmHardwareServer::IIncomingCommandSharedPtr)

#endif // IINCOMINGCOMMAND_H
