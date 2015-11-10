#ifndef MESSAGEINCOMINGCOMMANDIMPL_H
#define MESSAGEINCOMINGCOMMANDIMPL_H


#include "messageincomingcommand.h"


namespace ArmHardwareClient {

class MessageIncomingCommandImpl : public MessageIncomingCommand
{
    Q_OBJECT
public:
    explicit MessageIncomingCommandImpl(
            const QString &hardwareId,
            const QString &message,
            QObject *parent = 0);

    // MessageIncomingCommand interface
public:
    virtual QString hardwareId() const override;
    virtual QString message() const override;

private:
    const QString mHardwareId;
    const QString mMessage;
};

}

#endif // MESSAGEINCOMINGCOMMANDIMPL_H
