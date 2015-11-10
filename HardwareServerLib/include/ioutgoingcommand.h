#ifndef IOUTGOINGCOMMAND_H
#define IOUTGOINGCOMMAND_H

#include "hardwareserver_global.h"

#include <QObject>

namespace ArmHardwareServer {

class HARDWARESERVERSHARED_EXPORT IOutgoingCommand : public QObject
{
    Q_OBJECT
public:
    explicit IOutgoingCommand(QObject *parent = 0)  : QObject(parent) {}

    virtual quint16 code() const = 0;
    virtual QByteArray payload() const = 0;
};

}

#endif // IOUTGOINGCOMMAND_H
