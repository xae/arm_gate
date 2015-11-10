#ifndef IINCOMMINGCOMMAND_H
#define IINCOMMINGCOMMAND_H

#include "hardwareclient_global.h"

#include <QObject>
#include <QMetaType>
#include <QSharedPointer>


namespace ArmHardwareClient {

class HARDWARE_CLIENTSHARED_EXPORT IIncomingCommand : public QObject
{
    Q_OBJECT

public:
    enum Type {
        NotifyStatusChange = 0x0001,
        HardwareStatusChange = 0x0002,
        Message = 0x0003
    };

public:
    IIncomingCommand(QObject *parent = 0) : QObject(parent) {}

    virtual Type commandType() const = 0;
};

typedef QSharedPointer<IIncomingCommand> IIncomingCommandSharedPtr;

}

Q_DECLARE_METATYPE(ArmHardwareClient::IIncomingCommandSharedPtr)

#endif // IINCOMMINGCOMMAND_H
