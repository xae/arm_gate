#ifndef HARDWARECLIENTTESTMAINWINDOW_H
#define HARDWARECLIENTTESTMAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "inotifycontextaccess.h"

namespace Ui {
class HardwareClientTestMainWindow;
}

class HardwareClientTestMainWindow : public QMainWindow, public INotifyContextAccess
{
    Q_OBJECT

public:
    explicit HardwareClientTestMainWindow(QWidget *parent = 0);
    ~HardwareClientTestMainWindow();
    virtual ArmHardwareClient::NotifyContext notifyContext() const override;
    virtual QStringList hardwaresIds() const override;
    virtual quint16 serverPort() const override;
    virtual QString serverAddress() const override;

signals:
    void connectOrDisconnectRequested();
    void startNotify();
    void requestHardwareStatuses();

public Q_SLOTS:
    void setConnectDisconnectButtonText(const QString &text);

private Q_SLOTS:
    void addHardwareIdButtonPushed();
    void removeHardwareIdButtonPushed();
    void startNotifyButtonPushed();
    void requestHardwareStatusesButtonPushed();
    void connectDisconnectButtonPushed();

    void logMessage(const QString &message);

private:
    Ui::HardwareClientTestMainWindow *ui;
    QStandardItemModel mModel;
};

#endif // HARDWARECLIENTTESTMAINWINDOW_H
