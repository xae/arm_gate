#ifndef HARDWARESERVERTESTAPPMAINWINDOW_H
#define HARDWARESERVERTESTAPPMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "hardwareinfo.h"

namespace Ui {
class HardwareServerTestAppMainWindow;
}

class Model : public QStandardItemModel
{
    Q_OBJECT
public:
    Model(QObject *parent = 0);
    virtual ~Model();

    void addHardware(const HardwareServerTestApp::HardwareInfo &hardwareInfo);
    void deleteHardware(const HardwareServerTestApp::HardwareInfo &hardwareInfo);
    void setHardwareState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState);
    void setNotifyState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState);
    HardwareServerTestApp::HardwareInfo hardwareInfo(const QModelIndex &index);

signals:
    void newHardwareSetting(const HardwareServerTestApp::HardwareInfo &hardwareSetting);

private Q_SLOTS:
    void onItemChanged(QStandardItem *item);

};

class HardwareServerTestAppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HardwareServerTestAppMainWindow(QWidget *parent = 0);
    ~HardwareServerTestAppMainWindow();

public Q_SLOTS:
    void setHardwareState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState);
    void setNotifyState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState);
    void addHardware(const HardwareServerTestApp::HardwareInfo &hardwareInfo);

    void logMessage(const QString &message);

    void onTreeItemActivated(const QModelIndex &index);

    void onNotifyStateComboboxIndexChanged(const int index);
    void onHardwareStateComboboxIndexChanged(const int index);

    void onUuidChanged(const QString &uuid);
    void onDurationChanged(const int duration);

    void onSendMessageButtonPushed();
    void onSendCurrentNotifyStatusButtonPushed();
    void onSendCurrentHardwareStatusButtonPushed();
    void onCustomContextMenuRequested(const QPoint &pos);

    void deleteHardwareActionTriggered();
    void addHardwareActionTriggered();

signals:
    void message(const QString &globalId, const QString &message);
    void notifyStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState);
    void hardwareStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState);
    void newHardwareSetting(const HardwareServerTestApp::HardwareInfo &hardwareSetting);
    void removeHardware(const HardwareServerTestApp::HardwareInfo &hardwareSetting);

private:
    void fillCurrentHardwareInfo (const HardwareServerTestApp::HardwareInfo &hardwareInfo);
    void setCurrentNotifyStatus(const HardwareServerTestApp::HardwareInfo::NotifyState &state);
    void setCurrentHardwareStatus(const HardwareServerTestApp::HardwareInfo::HardwareState &state);

private:
    Ui::HardwareServerTestAppMainWindow *ui;
    Model mModel;
};

#endif // HARDWARESERVERTESTAPPMAINWINDOW_H
