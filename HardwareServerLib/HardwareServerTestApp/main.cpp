#include "hardwareservertestappmainwindow.h"
#include <QApplication>
#include <QStringList>
#include <QDebug>
#include <QUuid>

#include "hardwareinfo.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<HardwareServerTestApp::HardwareInfo>("HardwareServerTestApp::HardwareInfo");
    qRegisterMetaType<HardwareServerTestApp::HardwareInfo::HardwareState>("HardwareServerTestApp::HardwareInfo::HardwareState");
    qRegisterMetaType<HardwareServerTestApp::HardwareInfo::NotifyState>("HardwareServerTestApp::HardwareInfo::NotifyState");

    quint16 port = 45874;
    QStringList args = a.arguments();
    if (args.size() > 1) {
        bool ok;
        quint16 tmp = args.at(1).toUInt(&ok);
        if (ok) {
            port = tmp;
        }
    }

    using namespace HardwareServerTestApp;
    HardwareServerTestAppMainWindow w;
    Server s(port);

    QObject::connect(&s, SIGNAL(logMessage(QString)), &w, SLOT(logMessage(QString)));
    QObject::connect(&w, SIGNAL(hardwareStateChange(QString,HardwareServerTestApp::HardwareInfo::HardwareState)),
                     &s, SIGNAL(hardwareStateChange(QString,HardwareServerTestApp::HardwareInfo::HardwareState)));
    QObject::connect(&w, SIGNAL(message(QString,QString)),
                     &s, SIGNAL(message(QString,QString)));
    QObject::connect(&w, SIGNAL(newHardwareSetting(HardwareServerTestApp::HardwareInfo)),
                     &s, SIGNAL(newHardwareSetting(HardwareServerTestApp::HardwareInfo)));
    QObject::connect(&w, SIGNAL(notifyStateChange(QString,HardwareServerTestApp::HardwareInfo::NotifyState)),
                     &s, SIGNAL(notifyStateChange(QString,HardwareServerTestApp::HardwareInfo::NotifyState)));
    QObject::connect(&w, SIGNAL(removeHardware(HardwareServerTestApp::HardwareInfo)),
                     &s, SIGNAL(removeHardware(HardwareServerTestApp::HardwareInfo)));



    w.show();

    return a.exec();
}
