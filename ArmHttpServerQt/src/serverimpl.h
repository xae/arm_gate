#ifndef SERVERIMPL_H
#define SERVERIMPL_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include <qhttpserver.h>
#include "fileinfo.h"

class ServerImpl : public QObject
{
    Q_OBJECT
public:
    ServerImpl(const quint16 port, QObject *parent = nullptr);
    Q_INVOKABLE bool listen();
    Q_INVOKABLE void stop();

    void handleRegisterFiles(QHttpRequest *req, QHttpResponse *resp);
    void handleGetFile(QHttpResponse *resp, const QString &fileHash);
    void handleRoot(QHttpResponse *resp);

signals:
    void serverStatus(bool started);


private:
    void setContentType(QHttpResponse *resp, const QString &contentType);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
    void checkFiles();

private:
    QHttpServer *mServer;
    const quint16 mPort;
    QTimer *mTimer;
    QHash<QString, FileInfo> mRegisteredFiles;

};

#endif // SERVERIMPL_H
