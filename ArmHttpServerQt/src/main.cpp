#include <QDesktopServices>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QStringList>
#include <QThread>
#include <QDebug>
#include <QMenu>
#include <QUrl>

#include "serverimpl.h"
#include "serverrunner.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread thread;
    QStringList args = a.arguments();
    quint16 port = 8080;
    if (args.count() > 1) {
        bool ok;
        port = args.at(1).toUInt(&ok);
        if (!ok) {
            port = 8080;
        }
    }
    QSystemTrayIcon icon(QIcon(":/resources/icon.ico"));
    icon.show();
    icon.showMessage(QObject::tr("Information"), QObject::tr("Trying to start file server on %0 port").arg(port));
    ServerRunner runner(port);
    QObject::connect(&runner,&ServerRunner::serverStatus,
                     [&icon, port](bool serverStatus) {
        if (serverStatus) {
            icon.setToolTip(QObject::tr("File server started on %0 port").arg(port));
            icon.showMessage(QObject::tr("Information"), icon.toolTip());
        } else {
            icon.showMessage(QObject::tr("Error"), QObject::tr("Falied to start file server on %0 port").arg(port));
            QTimer::singleShot(3000, qApp, &QApplication::quit);
        }
    });

    QObject::connect(&icon, &QSystemTrayIcon::activated, [&icon, port](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            icon.showMessage(QObject::tr("Information"), icon.toolTip());
        } else if (reason == QSystemTrayIcon::Context) {
            QMenu *menu = new QMenu();
            QAction *action = menu->addAction(QObject::tr("Close ARM file server"));
            QObject::connect(action, &QAction::triggered, qApp, &QApplication::quit);
            action = menu->addAction(QObject::tr("Show information about registered files"));
            QObject::connect(action, &QAction::triggered, [port]() {
               QDesktopServices::openUrl(QUrl::fromUserInput(QString("http://localhost:%0").arg(port)));

            });
            QObject::connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);

            menu->popup(QCursor::pos());
        }
    });
    thread.start();
    runner.moveToThread(&thread);
    runner.runServer();
    QObject::connect(qApp, &QApplication::aboutToQuit,
                     &thread, &QThread::quit);
    return a.exec();
}
