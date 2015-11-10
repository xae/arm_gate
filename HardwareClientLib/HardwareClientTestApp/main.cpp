#include "hardwareclienttestmainwindow.h"
#include <QApplication>

#include "worker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HardwareClientTestMainWindow w;
    Worker worker(&w);
    QObject::connect(&worker, SIGNAL(connectDisconnectButtonText(QString)),
                     &w, SLOT(setConnectDisconnectButtonText(QString)));
    QObject::connect(&worker, SIGNAL(logMessage(QString)),
                     &w, SLOT(logMessage(QString)));

    QObject::connect(&w, SIGNAL(connectOrDisconnectRequested()),
                     &worker, SLOT(connectDisconnect()));
    QObject::connect(&w, SIGNAL(requestHardwareStatuses()),
                     &worker, SLOT(requestStatuses()));
    QObject::connect(&w, SIGNAL(startNotify()),
                     &worker, SLOT(startNotify()));

    w.show();

    return a.exec();
}
