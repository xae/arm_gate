//-----------------------------------------//
#include "arms_manager.h"

#include <QList>

#include "hardwareserver.h"
#include "requesthardwarestatusincomingcommand.h"
#include "startnotifyincomingcommand.h"

#include "messageoutgoingcommand.h"
#include "hardwarestatuschangeoutgoingcommand.h"
#include "notifystatuschangeoutgoingcommand.h"

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
    assert(command);
    switch (command->commandCode())
    {
    case IIncomingCommand::StartNotify:
        handleStartNotifyCommand(command);
        break;
    case IIncomingCommand::RequestStatuses:
        handleRequestHardwareStatusIncomingCommand(command);
        break;
    default:
        break;
    }
}
//-----------------------------------------//
void ArmsManager::handleStartNotifyCommand(ArmHardwareServer::IIncomingCommandSharedPtr command)
{
//    emit logMessage(tr("Принята команда на запуск оповещения"));
    StartNotifyIncomingCommand* cmd = qobject_cast<StartNotifyIncomingCommand*>(command.data());
    if (cmd) {
        QStringList acceptedHardwares;
        QStringList hardwaresToNotify = cmd->hardwaresUuids();
        cmd->sendResponse(acceptedHardwares);
//        for (const auto &hi : toProcess) {
//            NotifyStatusChangeOutgoingCommand *cmd = new NotifyStatusChangeOutgoingCommand(hi.hardwareId,
//                                                                                           NotifyStatusChangeOutgoingCommand::Process,
//                                                                                           QDateTime::currentDateTime());
//            mServer->sendOutgoingCommand(cmd);
//            // планинуем отчет о завершении оповещения через HardwareInfo::notifyDuration секунд
//            EndNotifyCommandHelper *helper = new EndNotifyCommandHelper(mServer, hi.hardwareId);
//            QTimer::singleShot(hi.notifyDuration * 1000, helper, SLOT(endNotify()));
//        }
    }
}
//-----------------------------------------//
void ArmsManager::handleRequestHardwareStatusIncomingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command)
{
//    emit logMessage(tr("Принята команда на запрос состояния обороудования"));
    RequestHardwareStatusIncomingCommand* cmd = qobject_cast<RequestHardwareStatusIncomingCommand*>(command.data());
    if (cmd)
    {
        QStringList hardwares = cmd->requestedHardwaresIds();
        QList<HardwareInfo> hardwaresToProcess;
        if (hardwares.isEmpty()) {
//            hardwaresToProcess = mHardwares.values();
        } else {
//            QHashIterator<QString, HardwareInfo> it(mHardwares);
//            while (it.hasNext()) {
//                it.next();
//                if (hardwares.contains(it.value().hardwareId)) {
//                    hardwaresToProcess.append(it.value());
//                }
//            }
        }
        QList<ArmHardwareServer::HardwareStatus> statuses;
        for (const auto &hi : hardwaresToProcess) {
            ArmHardwareServer::HardwareStatus s;
            s.hardwareId = hi.hardwareId;
            s.status = hi.hardwareState == HardwareInfo::Ok ?
                        ArmHardwareServer::HardwareStatus::Ok :
                        ArmHardwareServer::HardwareStatus::Error;
            s.notifyStatus = hi.notifyState == HardwareInfo::Process ?
                        ArmHardwareServer::HardwareStatus::Process :
                        ArmHardwareServer::HardwareStatus::Wait;
            s.notifyStatusCheckTime = QDateTime::currentDateTime();
            s.statusCheckTime = s.notifyStatusCheckTime;
            statuses.append(s);
        }
        cmd->sendResponse(statuses);
    }
}
//-----------------------------------------//

