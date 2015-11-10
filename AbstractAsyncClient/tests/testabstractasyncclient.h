#ifndef TESTABSTRACTASYNCCLIENT
#define TESTABSTRACTASYNCCLIENT

#include "abstractasyncclient.h"
#include "simpletransport.h"

#include <QtTest>

class TestAbstractAsyncClient : public QObject
{
    Q_OBJECT

public:
    TestAbstractAsyncClient();

    /* helper slots */
    Q_INVOKABLE void onTransportDestroyed();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testProtocolCase1();
    void transportAutoDeleteTest();

private:
    void startEventLoop(const int delay);

private:
    AsyncClient::AbstractAsyncClient *mClient;
    SimpleTransport *mTransport;
    bool mTransportIsDeleted;
};

#endif // TESTABSTRACTASYNCCLIENT

