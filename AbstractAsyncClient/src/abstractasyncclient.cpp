#include "abstractasyncclient.h"
#include "abstractasyncclientprivate.h"

using namespace AsyncClient;

AbstractAsyncClient::AbstractAsyncClient(IAbstractTransport *transport, const QByteArray &signature,
        QObject *parent) :
    QObject(parent),
    d(new AbstractAsyncClientPrivate(transport, signature, this))
{

}

AbstractAsyncClient::~AbstractAsyncClient()
{

}

void AbstractAsyncClient::addCommad(const ICommandSharedPtr &cmd)
{
    d->addCommad(cmd);
}

void AbstractAsyncClient::writeResponse(const PayloadPacketSharedPtr &response)
{
    d->writeResponse(response);
}

bool AbstractAsyncClient::open()
{
    return d->open();
}

void AbstractAsyncClient::close()
{
    return d->close();
}

bool AbstractAsyncClient::isOpen() const
{
    return d->isOpen();
}

QByteArray AbstractAsyncClient::signature() const
{
    return d->signature();
}

QString AbstractAsyncClient::peerAddress() const
{
    if (d->mTransport) {
        return d->mTransport->peerAddres();
    } else {
        return QString("null");
    }
}

AbstractAsyncClient::ClientState AbstractAsyncClient::clientState() const
{
    return d->clientState();
}
