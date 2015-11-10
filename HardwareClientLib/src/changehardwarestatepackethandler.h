#ifndef CHANGEHARDWARESTATEPACKETHANDLER_H
#define CHANGEHARDWARESTATEPACKETHANDLER_H

#include "iincomingpackethandler.h"

namespace ArmHardwareClient {


class ChangeHardwareStatePacketHandler : public IIncommingPacketHandler
{
    // IIncommingPacketHandler interface
public:
    virtual IIncomingCommand *outgoingCommand(const QByteArray &packetPayload) override;

};

}
#endif // CHANGEHARDWARESTATEPACKETHANDLER_H
