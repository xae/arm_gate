#ifndef INOTIFYCONTEXTACCESS_H
#define INOTIFYCONTEXTACCESS_H

#include <startnotifyoutgoingcommand.h>

class INotifyContextAccess
{
public:
    virtual ~INotifyContextAccess() {}
    virtual ArmHardwareClient::NotifyContext notifyContext() const = 0;
    virtual QStringList hardwaresIds() const = 0;
    virtual quint16 serverPort() const = 0;
    virtual QString serverAddress() const = 0;

};

#endif // INOTIFYCONTEXTACCESS_H
