#include "tcptransport.h"
#include <QHostAddress>

using namespace AsyncClient;

TcpTransport::TcpTransport(const QString &host, int port, QObject *parent) :
    IAbstractTransport(parent),
    mTcpSocket(),
    mHost(host),
    mPort(port),
    /* 20 секунд для ожидания ответа */
    mTimeoutWaitDateValue(20000)
{

}

TcpTransport::TcpTransport(QTcpSocket *socket, QObject *parent) :
    IAbstractTransport(parent),
    mTcpSocket(QPointer<QTcpSocket>(socket)),
    mHost(QString()),
    mPort(-1),
    /* 20 секунд для ожидания ответа */
    mTimeoutWaitDateValue(20000)
{
    connectSocket();
    if (mTcpSocket->isOpen()){
        changeState(Connected);
    }
}

TcpTransport::~TcpTransport()
{
    close();
}

bool AsyncClient::TcpTransport::open()
{
    if (!mTcpSocket) {
        if (mHost.isEmpty() || mPort < 0) {
            return false;
        }
        mTcpSocket = QPointer<QTcpSocket>(new QTcpSocket());
        connectSocket();
        mTcpSocket->connectToHost(mHost, mPort);
        changeState(Connecting);
    }
    return true;
}

void AsyncClient::TcpTransport::close()
{
    changeState(DisconnectByUser);
    if (mTcpSocket) {
        if (mTcpSocket->isOpen()){
            mTcpSocket->close();
        }
        mTcpSocket->deleteLater();
    }
}

bool AsyncClient::TcpTransport::isOpen() const
{
    /*
    if (mTcpSocket){
        auto state = mTcpSocket->state();
        int x= 0;
    }
    */
    return mTcpSocket && mTcpSocket->state() == QAbstractSocket::ConnectedState;
}

QByteArray AsyncClient::TcpTransport::read(const int size)
{
    if (mTcpSocket){
        return mTcpSocket->read(size);
    }
    return QByteArray();
}

QByteArray AsyncClient::TcpTransport::readAll()
{
    if (mTcpSocket) {
        return mTcpSocket->readAll();
    }
    return QByteArray();
}

bool AsyncClient::TcpTransport::getChar(char *c)
{
    if (mTcpSocket){
        return mTcpSocket->getChar(c);
    }
    return false;
}

void AsyncClient::TcpTransport::write(const QByteArray &data)
{
    if (mTcpSocket){
        mTcpSocket->write(data);
    }
}

IAbstractTransport::TransportState AsyncClient::TcpTransport::state() const
{
    return mCurrentState;
}

int AsyncClient::TcpTransport::timeoutWaitDataValue() const
{
    return mTimeoutWaitDateValue;
}

void TcpTransport::changeState(IAbstractTransport::TransportState state)
{
    if (state != mCurrentState) {
        mCurrentState = state;
        emit stateChanged(state);
    }
}

void TcpTransport::connectSocket()
{
    if (mTcpSocket) {
        connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(onSocketError(QAbstractSocket::SocketError)));
        connect(mTcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
        connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        connect(mTcpSocket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    }
}

void TcpTransport::onSocketError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError){
        changeState(Disconnected);
    } else if (error == QAbstractSocket::ConnectionRefusedError) {
        changeState(ConnectionError);
    } else {
        changeState(TransportError);
    }
    QTcpSocket *soc = qobject_cast<QTcpSocket*>(sender());
    if (soc){
        soc->deleteLater();
    }
}

void TcpTransport::onConnected()
{
    changeState(Connected);
}

void TcpTransport::onDisconnected()
{
    changeState(Disconnected);
}

QString AsyncClient::TcpTransport::peerAddres() const
{
    if (mTcpSocket) {
        if (mTcpSocket->peerAddress() != QHostAddress::Null){
            return QString("%0:%1").arg(mTcpSocket->peerAddress().toString())
                    .arg(mTcpSocket->peerPort());
        }
    }
    return QString("null");

}
