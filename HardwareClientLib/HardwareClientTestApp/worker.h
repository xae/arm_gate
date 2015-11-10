#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>

#include "inotifycontextaccess.h"
#include "threadworker.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(INotifyContextAccess *notifyContextAccess);
    ~Worker();

signals:
    void connectDisconnectButtonText(const QString &text);
    void logMessage(const QString &message);

public Q_SLOTS:
    void connectDisconnect();
    void startNotify();
    void requestStatuses();

    void onConnectedToServer();
    void onDisconnectedFromServer();
    void onError();

private Q_SLOTS:
    void quit();

private:
    INotifyContextAccess *mNotifyContextAccess;
    ThreadWorker *mWorker;
    QThread mThread;



};

#endif // WORKER_H
