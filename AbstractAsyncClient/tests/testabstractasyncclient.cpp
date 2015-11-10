#include "testabstractasyncclient.h"
#include "simplecommand.h"

#include <QCoreApplication>

using namespace AsyncClient;

TestAbstractAsyncClient::TestAbstractAsyncClient()
{
}

void TestAbstractAsyncClient::initTestCase()
{
    mTransport = new SimpleTransport();
    mClient = new AbstractAsyncClient(mTransport, "simple", 0);
}

void TestAbstractAsyncClient::cleanupTestCase()
{
    delete mClient;
}

void TestAbstractAsyncClient::testProtocolCase1()
{
    mTransport->setTimeout(1);
    SimpleCommand *cmd = new SimpleCommand(mClient->signature());
    QByteArray payload = QByteArray::fromHex("0102030405060708090a");
    quint16 command = 0x0001;
    cmd->setPayload(payload);
    cmd->setCommand(command);
    auto sharedCmd = ICommandSharedPtr(cmd);
    mClient->addCommad(sharedCmd);
    PayloadPacketSharedPtr packet = PayloadPacketSharedPtr(new PayloadPacket(mClient->signature()));
    packet->setPacketPayload(payload);
    packet->setPacketId(cmd->packetId());
    packet->setCommand(0x0000);
    packet->writeBinaryPacket(mTransport);
    startEventLoop(3);
    QVERIFY(cmd->packetState());
}

void TestAbstractAsyncClient::transportAutoDeleteTest()
{
    auto t = new SimpleTransport();
    auto c = new AbstractAsyncClient(t, "ssss", 0);
    mTransportIsDeleted = false;
    connect(t, SIGNAL(destroyed()), SLOT(onTransportDestroyed()));
    delete c;
    qApp->processEvents();
    QVERIFY(mTransportIsDeleted);

}

void TestAbstractAsyncClient::startEventLoop(const int delay)
{
    QEventLoop el;
    QTimer::singleShot(delay, &el, SLOT(quit()));
    el.exec();
}

void TestAbstractAsyncClient::onTransportDestroyed()
{
    mTransportIsDeleted = true;
}

