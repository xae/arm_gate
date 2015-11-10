#ifndef PAYLOADPACKET_H
#define PAYLOADPACKET_H

#include "abstractasyncclient_global.h"

#include <QObject>
#include <QIODevice>
#include <QSharedPointer>

namespace AsyncClient {

class PayloadPacket;
class IAbstractTransport;
typedef QSharedPointer<PayloadPacket> PayloadPacketSharedPtr;

class ABSTRACTASYNCCLIENTSHARED_EXPORT PayloadPacket
{
public:

    enum PacketState {
        Ok,
        Incomplete,
        SignatureError,
        CrcError
    };

    PayloadPacket(const QByteArray &signature);
    ~PayloadPacket();

    void writeBinaryPacket(IAbstractTransport *ioDevice) const;
    QByteArray signature() const;

    QByteArray packetId() const;
    void setPacketId(const QString &packetId);
    void setPacketId(const QByteArray &packetId);

    QByteArray packetPayload() const;
    void setPacketPayload(const QByteArray &packetPayload);

    quint16 command() const;
    void setCommand(const quint16 &command);

    quint32 lenght() const;

    PacketState packetState() const;

    static PayloadPacket *packetFromRawData(const QByteArray &data, const QByteArray &signature);

private:
    static quint16 crc16(const char *data, quint32 lenght, quint16 initialCrc = 0x0000);
    quint16 calculatedCrc() const;
    static void initCrc16Table();
    static quint16 *createCrc16Table();
    static bool checkEndianness();

    static quint32 toUInt32(const char *data);
    static quint16 toUInt16(const char *data);

    static QByteArray fromUInt16(const quint16 value);
    static QByteArray fromUInt32(const quint32 value);

private:
    QByteArray mPacketId;
    const QByteArray mSignature;
    quint16 mCommand;
    QByteArray mPacketPayload;
    static quint16 *mCrcTable;
    static bool mIsBigEndian;
    PacketState mPacketState;
};

}

#endif // PAYLOADPACKET_H

