//-----------------------------------------//
#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "arms_manager.h"
#include "remote_devices_manager.h"

#include <cassert>
//-----------------------------------------//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::Window |
        Qt::WindowMinimizeButtonHint |
        Qt::WindowStaysOnTopHint |
        Qt::CustomizeWindowHint),
    ui_(std::unique_ptr<Ui::MainWindow>(new Ui::MainWindow)),
    armsManager_(std::unique_ptr<ArmsManager>(new ArmsManager())),
    remoteDevicesManager_(std::unique_ptr<RemoteDevicesManager>(new RemoteDevicesManager()))
{
    ui_->setupUi(this);
    setWindowTitle(QApplication::applicationName());
    setWindowIcon(QApplication::windowIcon());

    // tray
    connect(&tray_,
            SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,
            SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));

    menu_.addAction(tr("Открыть"),this,SLOT(onShow()))->setVisible(false);
    menu_.setDefaultAction(menu_.addAction(tr("Спрятать"),this,SLOT(onHide())));
    menu_.addSeparator();
//    menu_.addAction(tr("Лог"),this,SLOT(onShowLog()));
//
    menu_.addAction(tr("Выход"),this,SLOT(onExit()));
    menu_.addSeparator();
    tray_.setContextMenu(&menu_);

//    movia_.stop();
//    movia_.setFileName(":/new/list/LAN.png");
//    movia_.start();
//    tray_.setIcon(QIcon(movia_.currentPixmap()));
    tray_.setIcon(QIcon(":/new/list/LAN.png"));
    tray_.show();
    onHide();

    assert(armsManager_);
    armsManager_->init();
    tray_.showMessage(QApplication::applicationName(), tr("Программа запущена: порт 2015"));
}
//-----------------------------------------//
MainWindow::~MainWindow()
{

}
//-----------------------------------------//
void MainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        if(isVisible() == true)
        {
            onHide();
        }
        else
        {
            onShow();
        }
    }
}
//-----------------------------------------//
void MainWindow::onShowLog()
{

}
//-----------------------------------------//
void MainWindow::onShow()
{
    menu_.actions().value(0)->setVisible(false);
    menu_.actions().value(1)->setVisible(true);
    menu_.setDefaultAction(menu_.actions().value(1));
    showNormal();
    activateWindow();
}
//-----------------------------------------//
void MainWindow::onHide()
{
    menu_.actions().value(1)->setVisible(false);
    menu_.actions().value(0)->setVisible(true);
    menu_.setDefaultAction(menu_.actions().value(0));
    hide();
}
//-----------------------------------------//
void MainWindow::onExit()
{
    if(QMessageBox::question(this,
        windowTitle(),
        tr("Закрыть программу?"),
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}
//-----------------------------------------//
