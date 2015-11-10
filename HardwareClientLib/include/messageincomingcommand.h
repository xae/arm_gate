#ifndef MESSAGEINCOMINGCOMMAND_H
#define MESSAGEINCOMINGCOMMAND_H

#include "hardwareclient_global.h"
#include "iincomingcommand.h"

namespace ArmHardwareClient {

class HARDWARE_CLIENTSHARED_EXPORT MessageIncomingCommand : public IIncomingCommand
{
    Q_OBJECT
public:
    explicit MessageIncomingCommand(QObject *parent = 0);

    // IIncomingCommand interface
public:
    virtual Type commandType() const override;

    virtual QString hardwareId() const = 0;
    virtual QString message() const = 0;
};

}
#endif // MESSAGEINCOMINGCOMMAND_H
