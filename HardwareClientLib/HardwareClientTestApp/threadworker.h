#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QStringList>
#include <QObject>
#include <QMutex>

#include <hardwareclient.h>
#include <startnotifyoutgoingcommand.h>
#include <requesthardwarestatusoutgoingcommand.h>

class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = 0);
    Q_INVOKABLE void setServerProperties(const QString &address, const quint16 port);
    Q_INVOKABLE bool connectToServer();
    Q_INVOKABLE void close();
    Q_INVOKABLE bool isOpen() const;
    Q_INVOKABLE void startNotify();
    Q_INVOKABLE void requsestStatuses(const QStringList &harwaresIds);
    void setNotifyCurrentNotifyContext(const QStringList &hardwaresIds, const ArmHardwareClient::NotifyContext &context);
    void setHardwaresIdsForRequestStatuses(const QStringList &hardwaresIds);


signals:
    void logMessage(const QString &message);

    void connectedToServer();
    void disconnectedFromServer();
    void error();

public slots:
    void onIncomingCommand(ArmHardwareClient::IIncomingCommandSharedPtr command);
    void onConnectedToServer();
    void onDisconnectedFromServer();
    void onError();

    void onRequestStatusesCommandDone(const bool success);
    void onStartNotifyCommandDone(const bool success);

private:
    void handleChangeNotifyStatusCommand(ArmHardwareClient::IIncomingCommandSharedPtr command);
    void handleMessageCommand(ArmHardwareClient::IIncomingCommandSharedPtr command);
    void handleChangeHardwareStatusCommand(ArmHardwareClient::IIncomingCommandSharedPtr command);

private:
    QMutex mMutex;
    QStringList mHardwaresIdsForStartNotifyCommand;
    ArmHardwareClient::NotifyContext mContext;
    QString mServerAddres;
    quint16 mServerPort;
    ArmHardwareClient::HardwareClient *mClient;

};

#endif // THREADWORKER_H
