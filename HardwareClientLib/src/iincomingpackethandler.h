#ifndef IINCOMINGPACKETHANDLER_H
#define IINCOMINGPACKETHANDLER_H

#include "iincomingcommand.h"

namespace ArmHardwareClient {


class IIncommingPacketHandler
{
public:
    virtual ~IIncommingPacketHandler() {}
    virtual IIncomingCommand *outgoingCommand(const QByteArray &packetPayload) = 0;
};

}

#endif // IINCOMINGPACKETHANDLER_H
