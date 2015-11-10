#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H

#include <QObject>

namespace HardwareServerTestApp {

struct HardwareInfo
{
    enum HardwareState {
        Ok,
        Error
    };

    enum NotifyState {
        Wait,
        Process
    };

    QString globalId;
    QString hardwareId;
    QString description;
    QString message;
    int notifyDuration;
    HardwareState hardwareState;
    NotifyState notifyState;
};

}

Q_DECLARE_METATYPE(HardwareServerTestApp::HardwareInfo)
Q_DECLARE_METATYPE(HardwareServerTestApp::HardwareInfo::HardwareState)
Q_DECLARE_METATYPE(HardwareServerTestApp::HardwareInfo::NotifyState)

#endif // HARDWAREINFO_H
