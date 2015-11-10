#ifndef ICOMMAND
#define ICOMMAND

#include "abstractasyncclient_global.h"
#include "payloadpacket.h"

#include <QObject>

namespace AsyncClient {

class ICommand;

typedef QSharedPointer<ICommand> ICommandSharedPtr;

class ABSTRACTASYNCCLIENTSHARED_EXPORT ICommand : public QObject {
    Q_OBJECT
public:
    enum Error {
        NoError = 0,
        TimeoutWaitResponseError,
        WhileSendingError,
        NetworkError
    };

    ICommand(QObject *parent) : QObject(parent) {}
    virtual ~ICommand() {}
    virtual PayloadPacketSharedPtr outgoingPacket() const = 0;
    virtual void done(const PayloadPacketSharedPtr &responsePacket) = 0;
    virtual void error(Error errorCode) = 0;

signals:
    void message(const QString &message);
};

}
#endif // ICOMMAND

