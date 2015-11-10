#ifndef IOUTGOINGCOMMAND_H
#define IOUTGOINGCOMMAND_H

#include "hardwareclient_global.h"

#include <QObject>
#include <QMetaType>
#include <QSharedPointer>

namespace ArmHardwareClient {

class HARDWARE_CLIENTSHARED_EXPORT IOutgoingCommand : public QObject
{
    Q_OBJECT

public:
    enum Type {
        StartNotify = 0x0001,
        RequestHardwareStates = 0x0002
    };

public:
    explicit IOutgoingCommand(QObject *parent = 0) :
        QObject(parent) {}

    virtual Type commandType() const = 0;
    virtual QByteArray payload() const = 0;

public Q_SLOTS:
    void onDone() {doneImpl();}
    void onError() {errorImpl();}

protected:
    virtual void doneImpl() = 0;
    virtual void errorImpl() = 0;

signals:
    void finish(bool success);

public slots:

};

typedef QSharedPointer<IOutgoingCommand> IOutgoingCommandSharedPtr;

}

Q_DECLARE_METATYPE(ArmHardwareClient::IOutgoingCommandSharedPtr)



#endif // IOUTGOINGCOMMAND_H
