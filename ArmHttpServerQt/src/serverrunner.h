#ifndef SERVERRUNNER_H
#define SERVERRUNNER_H

#include <QObject>
#include <QThread>

#include "serverimpl.h"

class ServerRunner : public QObject
{
    Q_OBJECT
public:
    explicit ServerRunner(const quint16 port, QObject *parent = 0);
    Q_INVOKABLE void runServer();

    void onAboutToQuit();

signals:
    void serverStatus(bool started);

public slots:

private:
    ServerImpl *mServer;
    QThread mThread;
    const quint16 mPort;
};

#endif // SERVERRUNNER_H
