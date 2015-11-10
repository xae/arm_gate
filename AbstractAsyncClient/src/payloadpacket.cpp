#include "payloadpacket.h"
#include "iabstracttransport.h"

#include <QUuid>

using namespace AsyncClient;

quint16 *PayloadPacket::mCrcTable = PayloadPacket::createCrc16Table();
bool PayloadPacket::mIsBigEndian = PayloadPacket::checkEndianness();

/*!
 * \class PayloadPacket
 *
 * \brief Представляе функционал формирования пакета для обмена
 * между процессами.
 *
 * Формат пакета: сигнатура пакета; далее 4 байта (big endian format),
 * отвечающие за длинну ВСЕГО пакета, включая СRC; 32 байта - идентификатор
 * пакета, тип команды (2 байта), затем полезные данные, и в конце
 * 2 байта CRC16 (полином 0xa001)
 */

/*!
 * \brief PayloadPacket::PayloadPacket
 * \param signature Сигнатура пакета
 */

PayloadPacket::PayloadPacket(const QByteArray &signature) :
    mSignature(signature),
    mCommand(0x0000),
    mPacketState(Ok)
{
    setPacketId(QUuid::createUuid().toString().remove("-").remove("{").remove("}"));
}

PayloadPacket::~PayloadPacket()
{

}

void PayloadPacket::writeBinaryPacket(IAbstractTransport *ioDevice) const
{
    QByteArray rawLenght = fromUInt32(lenght());
    QByteArray rawCommand = fromUInt16(mCommand);
    quint16 crc16 = calculatedCrc();
    ioDevice->write(mSignature);
    ioDevice->write(rawLenght);
    ioDevice->write(mPacketId);
    ioDevice->write(rawCommand);
    ioDevice->write(mPacketPayload);
    ioDevice->write(fromUInt16(crc16));
}
QByteArray PayloadPacket::packetId() const
{
    return mPacketId;
}

void PayloadPacket::setPacketId(const QString &packetId)
{
    setPacketId(packetId.toLocal8Bit());

}

void PayloadPacket::setPacketId(const QByteArray &packetId)
{
    if (packetId.size() > 0x20){
        mPacketId = packetId.left(0x20);
    } else if (packetId.size() < 20) {
        mPacketId = QByteArray(0x20, 0x20);
        mPacketId.replace(0, packetId.size(), packetId);
    } else {
        mPacketId = packetId;
    }
}

quint16 PayloadPacket::calculatedCrc() const
{
    QByteArray rawLenght = fromUInt32(lenght());
    QByteArray rawCommand = fromUInt16(mCommand);
    quint16 crc = crc16(signature().constData(), signature().length());
    crc = crc16(rawLenght.constData(), rawLenght.size(), crc);
    crc = crc16(mPacketId.constData(), 0x20, crc);
    crc = crc16(rawCommand.constData(), rawCommand.length(), crc);
    crc = crc16(mPacketPayload.constData(), mPacketPayload.length(), crc);
    return crc;
}

quint16 PayloadPacket::crc16(const char *data, quint32 lenght, quint16 initialCrc)
{
    for (quint32 i = 0; i < lenght; ++ i) {
        initialCrc = (initialCrc >> 8) ^ mCrcTable[(initialCrc & 0xff) ^ (quint8)data[i]];
    }
    return initialCrc;
}

void PayloadPacket::initCrc16Table()
{
    mCrcTable = createCrc16Table();
}

quint16 *PayloadPacket::createCrc16Table()
{
    quint16 *crcTable = new quint16[0x100];
    for (quint16 i = 0; i < 256; ++i) {
        quint16 temp = i;
        quint16 value = 0x0000;
        for (quint16 x = 0; x < 8; ++x) {
            if (((temp ^ value) & 0x0001) != 0) {
                value = ((value >> 1) ^ 0xa001);
            } else {
                value >>= 1;
            }
            temp >>= 1;
        }
        crcTable[i] = value;
    }
    return crcTable;
}

bool PayloadPacket::checkEndianness ()
{
    quint32 isOne = 0x00000001;
    quint8 isOneByte = *(quint8*)&isOne;
    return isOneByte == 0x01;
}

quint32 PayloadPacket::toUInt32(const char *data)
{
    if (mIsBigEndian) {
        return *(quint32*)data;
    } else {
        return (quint32)(data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24);
    }
}

quint16 PayloadPacket::toUInt16(const char *data)
{
    if (mIsBigEndian) {
        return *(quint16*)data;
    } else {
        return (quint16)(data[0] | data[1] << 8);
    }
}

QByteArray PayloadPacket::fromUInt16(const quint16 value)
{
    if (mIsBigEndian) {
        return QByteArray((char*)&value, 0x02);
    } else {
        QByteArray retVal(0x02, (char)0x00);
        retVal[0] = (char) value;
        retVal[1] = (char) (value >> 8);
        return retVal;
    }
}

QByteArray PayloadPacket::fromUInt32(const quint32 value)
{
    if (mIsBigEndian) {
        return QByteArray((char*)&value, 0x04);
    } else {
        QByteArray retVal(0x04, (char)0x00);
        retVal[0] = (char) value;
        retVal[1] = (char) (value >> 8);
        retVal[2] = (char) (value >> 16);
        retVal[3] = (char) (value >> 24);
        return retVal;
    }
}
quint16 PayloadPacket::command() const
{
    return mCommand;
}

void PayloadPacket::setCommand(const quint16 &command)
{
    mCommand = command;
}

quint32 PayloadPacket::lenght() const
{
    return 40 + mSignature.length() + mPacketPayload.length();
}

PayloadPacket::PacketState PayloadPacket::packetState() const
{
    return mPacketState;
}

PayloadPacket* PayloadPacket::packetFromRawData(const QByteArray &data, const QByteArray &signature)
{
    PayloadPacket *retPacket = new PayloadPacket(signature);
    retPacket->mPacketState = Incomplete;
    quint32 size = data.size();
    quint32 signatureSize = signature.size();
    quint32 sizeDataOffset = signature.size();
    if (size < signatureSize) {
        return retPacket;
    }
    if (data.left(signature.size()) != signature) {
        retPacket->mPacketState = SignatureError;
        return retPacket;
    }
    if (size < signatureSize + 40) {
        return retPacket;
    }
    quint32 expectedSize = toUInt32(data.constData() + sizeDataOffset);
    quint32 packetIdDataOffset = sizeDataOffset + 0x04;
    quint32 commandDataOffset = packetIdDataOffset + 0x20;
    quint32 packetPayloadOffset = commandDataOffset + 0x02;
    quint32 crcDataOffset = expectedSize - 0x02;
    quint32 packetPayloadSize = crcDataOffset - packetPayloadOffset;

    if (size < expectedSize) {
        return retPacket;
    }
    retPacket->setCommand(toUInt16(data.constData() + commandDataOffset));
    retPacket->setPacketId(data.mid(packetIdDataOffset, 0x20));
    retPacket->setPacketPayload(data.mid(packetPayloadOffset, packetPayloadSize));
    quint16 expectedCrc = toUInt16(data.constData() + crcDataOffset);
    quint16 realCrc = retPacket->calculatedCrc();
    retPacket->mPacketState = expectedCrc == realCrc ? Ok : CrcError;
    return retPacket;
}

QByteArray PayloadPacket::packetPayload() const
{
    return mPacketPayload;
}

void PayloadPacket::setPacketPayload(const QByteArray &packetPayload)
{
    mPacketPayload = packetPayload;
}


QByteArray PayloadPacket::signature() const
{
    return mSignature;
}
