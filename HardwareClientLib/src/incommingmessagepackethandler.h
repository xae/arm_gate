#ifndef INCOMMINGMESSAGEPACKETHANDLER_H
#define INCOMMINGMESSAGEPACKETHANDLER_H

#include "iincomingpackethandler.h"

namespace ArmHardwareClient {

class IncommingMessagePacketHandler : public IIncommingPacketHandler
{
    // IIncommingPacketHandler interface
public:
    virtual IIncomingCommand *outgoingCommand(const QByteArray &packetPayload) override;

};

}
#endif // INCOMMINGMESSAGEPACKETHANDLER_H
