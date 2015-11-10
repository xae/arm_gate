#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QSystemTrayIcon>
#include <QMenu>

#include <memory>

namespace Ui
{
    class MainWindow;
}

class ArmsManager;
class RemoteDevicesManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
    void onShowLog();
    void onShow();
    void onHide();
    void onExit();

    void onClientConnected(QString ip);
    void onClientDisconnected(QString ip);


private:
    const std::unique_ptr<Ui::MainWindow> ui_;
    const std::unique_ptr<ArmsManager> armsManager_;
    const std::unique_ptr<RemoteDevicesManager> remoteDevicesManager_;

    QSystemTrayIcon tray_;
    QMenu menu_;
    QMovie movia_;
};

#endif // MAINWINDOW_H
