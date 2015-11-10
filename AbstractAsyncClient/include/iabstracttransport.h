#ifndef IABSTRACTTRANSPORT
#define IABSTRACTTRANSPORT

#include "abstractasyncclient_global.h"

#include <QObject>

namespace AsyncClient {
    class ABSTRACTASYNCCLIENTSHARED_EXPORT IAbstractTransport : public QObject {
        Q_OBJECT
    public:
        enum TransportState {
            Disconnected,
            Connected,
            Connecting,
            ConnectionError,
            TransportError,
            DisconnectByUser
        };

    public:
        IAbstractTransport(QObject *parent = 0) : QObject(parent) {}
        virtual bool open() = 0;
        virtual void close() = 0;
        virtual bool isOpen() const = 0;
        virtual QByteArray read(const int size) = 0;
        virtual QByteArray readAll() = 0;
        virtual bool getChar(char *c) = 0;
        virtual void write(const QByteArray &data) = 0;
        virtual IAbstractTransport::TransportState state() const = 0;
        virtual int timeoutWaitDataValue() const = 0;
        virtual QString peerAddres() const = 0;

    signals:
        void stateChanged(const IAbstractTransport::TransportState stateChanged);
        void readyRead();
    };
}

#endif // IABSTRACTTRANSPORT

