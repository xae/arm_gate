//-----------------------------------------//
#pragma once

#include <QObject>
#include <QTcpSocket>

#include <memory>
#include <map>
//-----------------------------------------//
class TcpServer;
class ArmsManager : public QObject
{
    Q_OBJECT
public:
    explicit ArmsManager(QObject *parent = 0);
    ~ArmsManager();

public:
    void init();

signals:
    void clientConnected(QString ip);
    void clientDisconnected(QString ip);

private slots:
    void onClientConnected(int handle);
    void onClientDisconnected();

private:
    const std::unique_ptr<TcpServer> tcpServer_;

    struct ClientInfo
    {
        QString ip;
        int handle;
        std::shared_ptr<QTcpSocket> socket;
    };
    std::map<int, ClientInfo> sockets_;
};
//-----------------------------------------//

