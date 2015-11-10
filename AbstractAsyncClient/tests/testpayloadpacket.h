#ifndef TESTPAYLOADPACKET
#define TESTPAYLOADPACKET

#include <QString>
#include <QtTest>
#include <QWaitCondition>
#include <QMutex>

#include "payloadpacket.h"
#include "simpletransport.h"

class TestPayloadPacket : public QObject
{
    Q_OBJECT

public:
    TestPayloadPacket();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void PayloadPacketCase();

private:
    QByteArray mSignature;
};

#endif // TESTPAYLOADPACKET

