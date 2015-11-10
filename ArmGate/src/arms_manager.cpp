//-----------------------------------------//
#include "arms_manager.h"

#include "hardwareserver.h"

#include <cassert>

using namespace ArmHardwareServer;

//-----------------------------------------//
ArmsManager::ArmsManager(QObject *parent) :
    QObject(parent),
    server_(std::unique_ptr<HardwareServer>(new HardwareServer(2015, QHostAddress::Any, this)))
{

}
//-----------------------------------------//
ArmsManager::~ArmsManager()
{
    assert(server_);
    server_->close();
}
//-----------------------------------------//
void ArmsManager::init()
{
    // start server
    assert(server_);
    connect(server_.get(),
            SIGNAL(incommingCommand(ArmHardwareServer::IIncomingCommandSharedPtr)),
            this,
            SLOT(onRecievedCommand(ArmHardwareServer::IIncomingCommandSharedPtr)));
    server_->startListen();
}
//-----------------------------------------//
void ArmsManager::onRecievedCommand(IIncomingCommandSharedPtr command)
{

}
//-----------------------------------------//

