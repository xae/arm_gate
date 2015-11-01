//-----------------------------------------//
#pragma once

#include <QTcpServer>
//-----------------------------------------//
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

public:
    void start(unsigned port);
    void stop();

signals:
    void clientConnected(int handle);

private:
    void incomingConnection(int handle) override;
};

//-----------------------------------------//
