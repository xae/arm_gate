//-----------------------------------------//
#include "arms_manager.h"

#include "settings.h"
#include "hardwareserver.h"
#include "requesthardwarestatusincomingcommand.h"
#include "startnotifyincomingcommand.h"

#include "messageoutgoingcommand.h"
#include "hardwarestatuschangeoutgoingcommand.h"
#include "notifystatuschangeoutgoingcommand.h"

#include <QList>
#include <cassert>
#include <QNetworkRequest>

using namespace ArmHardwareServer;

//-----------------------------------------//
ArmsManager::ArmsManager(QObject *parent) :
    QObject(parent),
    server_(new HardwareServer(Settings::value("server/port", 2015).toUInt(), QHostAddress::Any, nullptr)),
    networkManager_(new QNetworkAccessManager())
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
    if (StartNotifyIncomingCommand* cmd = qobject_cast<StartNotifyIncomingCommand*>(command.data()))
    {
        assert(server_);
        for(const auto& uuid: cmd->hardwaresUuids())
        {
            NotifyStatusChangeOutgoingCommand *ocmd = new NotifyStatusChangeOutgoingCommand(uuid,
                NotifyStatusChangeOutgoingCommand::Process,
                QDateTime::currentDateTime());
            server_->sendOutgoingCommand(ocmd);
        }

        QStringList acceptedHardwares;
        for(const auto& uuid: cmd->hardwaresUuids())
        {
            const auto children = Settings::get().childGroups();
            if(children.contains(uuid))
                acceptedHardwares.append(uuid);
        }

        assert(networkManager_);
        for(const auto& uuid: acceptedHardwares)
        {
            const QString url = tr("http://%1:%2/%3.htm").
                arg(Settings::value(uuid +"/ip").toString()).
                arg(Settings::value(uuid +"/port").toString()).
                arg(cmd->notifyContext().notifyId);
            networkManager_->get(QNetworkRequest(QUrl(url)));
        }
//        cmd->sendResponse(acceptedHardwares);
//        for (const auto &hi : toProcess) {

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

