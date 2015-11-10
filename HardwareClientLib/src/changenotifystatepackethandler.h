#ifndef CHANGENOTIFYSTATEPACKETHANDLER_H
#define CHANGENOTIFYSTATEPACKETHANDLER_H

#include "iincomingpackethandler.h"

namespace ArmHardwareClient {

class ChangeNotifyStatePacketHandler : public IIncommingPacketHandler
{
    // IIncommingPacketHandler interface
public:
    virtual IIncomingCommand *outgoingCommand(const QByteArray &packetPayload) override;
};

}

#endif // CHANGENOTIFYSTATEPACKETHANDLER_H
