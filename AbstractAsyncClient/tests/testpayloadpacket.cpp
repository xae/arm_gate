#include "testpayloadpacket.h"

TestPayloadPacket::TestPayloadPacket()
{
}

void TestPayloadPacket::initTestCase()
{
    mSignature = QByteArray::fromHex("01020304");
}

void TestPayloadPacket::cleanupTestCase()
{

}

void TestPayloadPacket::PayloadPacketCase()
{
    QVERIFY2(true, "Failure");
    using namespace AsyncClient;
    quint16 command = 0x1234;
    PayloadPacket packet(mSignature);
    packet.setCommand(command);
    QByteArray packetPayload = QByteArray::fromHex("1234567890");
    QByteArray packetId = QByteArray("12345678976554324");
    packet.setPacketId(packetId);
    packet.setPacketPayload(packetPayload);

    SimpleTransport res;
    res.open();
    packet.writeBinaryPacket(&res);
    res.close();
    QByteArray resBytearray = res.readAll();

    /* Обратно из данных в пакет */
    PayloadPacketSharedPtr deserializedPacket =
            PayloadPacketSharedPtr(PayloadPacket::packetFromRawData(resBytearray, mSignature));
    QCOMPARE(deserializedPacket->packetState(), PayloadPacket::Ok);
    QCOMPARE(deserializedPacket->lenght(), (quint32)resBytearray.length());

    QByteArray failedPacket = resBytearray;
    failedPacket[0] = 'l';
    /* Меняем сигнатуру на неверную */
    deserializedPacket = PayloadPacketSharedPtr(PayloadPacket::packetFromRawData(failedPacket, mSignature));
    QCOMPARE(deserializedPacket->packetState(), PayloadPacket::SignatureError);

    failedPacket = resBytearray;
    /* портим CRC */
    failedPacket[failedPacket.length() - 1] = failedPacket.at(failedPacket.length() - 1) + 1;
    deserializedPacket = PayloadPacketSharedPtr(PayloadPacket::packetFromRawData(failedPacket, mSignature));
    QCOMPARE(deserializedPacket->packetState(), PayloadPacket::CrcError);

    /* Пакет меньше 8 байт */
    failedPacket = mSignature + QByteArray("123");
    deserializedPacket = PayloadPacketSharedPtr(PayloadPacket::packetFromRawData(failedPacket, mSignature));
    QCOMPARE(deserializedPacket->packetState(), PayloadPacket::Incomplete);

    failedPacket = resBytearray;
    /* пакет меньше ожидаемого на 1 байт */
    failedPacket.resize(failedPacket.size() - 1);
    deserializedPacket = PayloadPacketSharedPtr(PayloadPacket::packetFromRawData(failedPacket, mSignature));
    QCOMPARE(deserializedPacket->packetState(), PayloadPacket::Incomplete);

    /* пакет больше ожидаемого */
    resBytearray.append("123456");
    deserializedPacket = PayloadPacketSharedPtr(PayloadPacket::packetFromRawData(resBytearray, mSignature));
    QCOMPARE(deserializedPacket->packetState(), PayloadPacket::Ok);
    QCOMPARE(deserializedPacket->packetPayload(), packetPayload);
    QCOMPARE(deserializedPacket->packetId().left(packetId.length()), packetId);
    QCOMPARE(deserializedPacket->command(), command);
}


