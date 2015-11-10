//-----------------------------------------//
#pragma once

#include <QObject>

class RemoteDevicesManager : public QObject
{
    Q_OBJECT
public:
    explicit RemoteDevicesManager(QObject *parent = 0);
    ~RemoteDevicesManager();

signals:

public slots:
};

//-----------------------------------------//
