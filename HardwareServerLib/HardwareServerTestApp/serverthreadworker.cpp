#include "serverthreadworker.h"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QDebug>

#include <messageoutgoingcommand.h>
#include <hardwarestatuschangeoutgoingcommand.h>
#include <notifystatuschangeoutgoingcommand.h>

#include <requesthardwarestatusincomingcommand.h>
#include <startnotifyincomingcommand.h>


using namespace HardwareServerTestApp;

ServerThreadWorker::ServerThreadWorker(const quint16 port, QObject *parent) :
    QObject(parent),
    mServer(nullptr),
    mPort(port)
{

}

void ServerThreadWorker::start()
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
        return;
    }
    mServer = new ArmHardwareServer::HardwareServer(mPort, QHostAddress::Any, this);
    if (mServer->startListen()) {
        emit logMessage(tr("Сервер запущен на порту %0").arg(mPort));
    } else {
        emit logMessage(tr("Не удалось запустить сервер на порту %0").arg(mPort));
        QTimer::singleShot(10000, qApp, SLOT(quit()));
    }
    connect(mServer, SIGNAL(incommingCommand(ArmHardwareServer::IIncomingCommandSharedPtr)),
            this, SLOT(onIncommingCommand(ArmHardwareServer::IIncomingCommandSharedPtr)));
}

void ServerThreadWorker::onMessage(const QString &globalId, const QString &message)
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mHardwares.value(globalId);
    if (hi.globalId == globalId) {
        emit logMessage(tr("Посылка сообщения '%0' клиентам от оборудования '%1'").arg(message, hi.description));
        using namespace ArmHardwareServer;
        auto cmd = new MessageOutgoingCommand(hi.hardwareId, message);
        mServer->sendOutgoingCommand(cmd);
    }
}

void ServerThreadWorker::onNotifyStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState)
{
    using namespace HardwareServerTestApp;
    const static QHash<HardwareInfo::NotifyState, ArmHardwareServer::NotifyStatusChangeOutgoingCommand::Status> statesBinding =
            []() {
        QHash<HardwareInfo::NotifyState, ArmHardwareServer::NotifyStatusChangeOutgoingCommand::Status> retVal;
        retVal[HardwareInfo::Wait] = ArmHardwareServer::NotifyStatusChangeOutgoingCommand::Wait;
        retVal[HardwareInfo::Process] = ArmHardwareServer::NotifyStatusChangeOutgoingCommand::Process;
        return retVal;
    }();


    HardwareInfo hi = mHardwares.value(globalId);
    if (hi.globalId == globalId) {
        emit logMessage(tr("Посылка комадны изменения статуса оповещения от оборудования '%1'").arg(hi.description));
        using namespace ArmHardwareServer;
        auto cmd = new NotifyStatusChangeOutgoingCommand(hi.hardwareId, statesBinding.value(newState), QDateTime::currentDateTime());
        mServer->sendOutgoingCommand(cmd);
    }
}

void ServerThreadWorker::onHardwareStateChange(const QString &globalId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState)
{
    using namespace HardwareServerTestApp;
    const static QHash<HardwareInfo::HardwareState, ArmHardwareServer::HardwareStatusChangeOutgoingCommand::Status> statesBinding =
            []() {
        QHash<HardwareInfo::HardwareState, ArmHardwareServer::HardwareStatusChangeOutgoingCommand::Status> retVal;
        retVal[HardwareInfo::Ok] = ArmHardwareServer::HardwareStatusChangeOutgoingCommand::Ok;
        retVal[HardwareInfo::Error] = ArmHardwareServer::HardwareStatusChangeOutgoingCommand::Error;
        return retVal;
    }();


    HardwareInfo hi = mHardwares.value(globalId);
    if (hi.globalId == globalId) {
        emit logMessage(tr("Посылка комадны изменения состояния оборудования '%1'").arg(hi.description));
        using namespace ArmHardwareServer;
        auto cmd = new HardwareStatusChangeOutgoingCommand(hi.hardwareId, statesBinding.value(newState), QDateTime::currentDateTime());
        mServer->sendOutgoingCommand(cmd);
    }
}

void ServerThreadWorker::onNewHardwareSetting(const HardwareServerTestApp::HardwareInfo &hardwareSetting)
{
    mHardwares[hardwareSetting.globalId] = hardwareSetting;

}

void ServerThreadWorker::onRemoveHardware(const HardwareServerTestApp::HardwareInfo &hardwareSetting)
{
    mHardwares.remove(hardwareSetting.globalId);
}

void ServerThreadWorker::onIncommingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command)
{
    switch (command->commandCode()) {
    case ArmHardwareServer::IIncomingCommand::StartNotify:
        handleStartNotifyCommand(command);
        break;
    case ArmHardwareServer::IIncomingCommand::RequestStatuses:
        handleRequestHardwareStatusIncomingCommand(command);
        break;
    default:
        break;
    }
}

void ServerThreadWorker::handleStartNotifyCommand(ArmHardwareServer::IIncomingCommandSharedPtr command)
{
    emit logMessage(tr("Принята команда на запуск оповещения"));
    using namespace ArmHardwareServer;
    StartNotifyIncomingCommand* cmd = qobject_cast<StartNotifyIncomingCommand*>(command.data());
    if (cmd) {
        QStringList acceptedHardwares;
        QList<HardwareInfo> toProcess;
        QStringList hardwaresToNotify = cmd->hardwaresUuids();
        QHashIterator<QString, HardwareInfo> it(mHardwares);
        while (it.hasNext()) {
            it.next();
            if (hardwaresToNotify.contains(it.value().hardwareId) && it.value().hardwareState == HardwareInfo::Ok) {
                acceptedHardwares.append(it.value().hardwareId);
                toProcess.append(it.value());
            }
        }
        cmd->sendResponse(acceptedHardwares);
        for (const auto &hi : toProcess) {
            NotifyStatusChangeOutgoingCommand *cmd = new NotifyStatusChangeOutgoingCommand(hi.hardwareId,
                                                                                           NotifyStatusChangeOutgoingCommand::Process,
                                                                                           QDateTime::currentDateTime());
            mServer->sendOutgoingCommand(cmd);
            // планинуем отчет о завершении оповещения через HardwareInfo::notifyDuration секунд
            EndNotifyCommandHelper *helper = new EndNotifyCommandHelper(mServer, hi.hardwareId);
            QTimer::singleShot(hi.notifyDuration * 1000, helper, SLOT(endNotify()));
        }
    }
}

void ServerThreadWorker::handleRequestHardwareStatusIncomingCommand(ArmHardwareServer::IIncomingCommandSharedPtr command)
{
    emit logMessage(tr("Принята команда на запрос состояния обороудования"));
    using namespace ArmHardwareServer;
    RequestHardwareStatusIncomingCommand* cmd = qobject_cast<RequestHardwareStatusIncomingCommand*>(command.data());
    if (cmd) {
        QStringList hardwares = cmd->requestedHardwaresIds();
        QList<HardwareInfo> hardwaresToProcess;
        if (hardwares.isEmpty()) {
            hardwaresToProcess = mHardwares.values();
        } else {
            QHashIterator<QString, HardwareInfo> it(mHardwares);
            while (it.hasNext()) {
                it.next();
                if (hardwares.contains(it.value().hardwareId)) {
                    hardwaresToProcess.append(it.value());
                }
            }
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

EndNotifyCommandHelper::EndNotifyCommandHelper(
        ArmHardwareServer::HardwareServer *server,
        const QString &hardwareId,
        QObject *parent) :
    QObject(parent),
    mServer(server),
    mHardwareId(hardwareId)
{

}

void EndNotifyCommandHelper::endNotify()
{
    using namespace ArmHardwareServer;
    NotifyStatusChangeOutgoingCommand *cmd = new NotifyStatusChangeOutgoingCommand(mHardwareId,
                                                                                   NotifyStatusChangeOutgoingCommand::Wait,
                                                                                   QDateTime::currentDateTime());
    mServer->sendOutgoingCommand(cmd);
    deleteLater();
}
