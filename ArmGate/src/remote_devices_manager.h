#ifndef REMOTE_DEVICES_MANAGER_H
#define REMOTE_DEVICES_MANAGER_H

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

#endif // REMOTE_DEVICES_MANAGER_H
