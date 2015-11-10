#include "threadworker.h"

#include <QThread>

#include <changenotifystatysincomingcommand.h>
#include <changehardwarestatusincomingcommand.h>
#include <messageincomingcommand.h>

ThreadWorker::ThreadWorker(QObject *parent) :
    QObject(parent),
    mClient(nullptr)
{

}

void ThreadWorker::setServerProperties(const QString &address, const quint16 port)
{
    mServerAddres = address;
    mServerPort = port;
}

bool ThreadWorker::connectToServer()
{
    if (QThread::currentThread() != thread()) {
        bool retVal;
        QMetaObject::invokeMethod(this, "connectToServer", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, retVal));
        return retVal;
    }
    if (mClient) {
        return false;
    }
    mClient = new ArmHardwareClient::HardwareClient(QHostAddress(mServerAddres), mServerPort);
    connect(mClient, SIGNAL(connectedToServer()),
            this, SLOT(onConnectedToServer()));
    connect(mClient, SIGNAL(disconnectedFromServer()),
            this, SLOT(onDisconnectedFromServer()));
    connect(mClient, SIGNAL(error()),
            this, SLOT(onError()));
    connect(mClient, SIGNAL(incomingCommand(ArmHardwareClient::IIncomingCommandSharedPtr)),
            this, SLOT(onIncomingCommand(ArmHardwareClient::IIncomingCommandSharedPtr)));

    emit logMessage(tr("Попытка соединения с сервером %0:%1")
                    .arg(mServerAddres, QString::number(mServerPort)));
    bool retVal = mClient->connectToServer();
    if (!retVal) {
        mClient->deleteLater();
        mClient = nullptr;
    }
    return retVal;
}

void ThreadWorker::close()
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "close", Qt::BlockingQueuedConnection);
        return;
    }
    if (mClient) {
        // mClient->close();
        mClient->deleteLater();
        mClient = nullptr;
    }
}

bool ThreadWorker::isOpen() const
{
    if (QThread::currentThread() != thread()) {
        bool retVal;
        QMetaObject::invokeMethod(const_cast<ThreadWorker*>(this), "isOpen", Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, retVal));
        return retVal;
    }
    return mClient && mClient->isOpen();
}

void ThreadWorker::startNotify()
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "startNotify", Qt::BlockingQueuedConnection);
        return;
    }
    if (isOpen()) {
        ArmHardwareClient::StartNotifyOutgoingCommand *cmd =
                new ArmHardwareClient::StartNotifyOutgoingCommand(mHardwaresIdsForStartNotifyCommand, mContext);
        connect(cmd, SIGNAL(finish(bool)), this, SLOT(onStartNotifyCommandDone(bool)));
        mClient->sendOutgoingCommand(cmd);
    }
}

void ThreadWorker::requsestStatuses(const QStringList &harwaresIds)
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "requsestStatuses", Qt::BlockingQueuedConnection, Q_ARG(QStringList, harwaresIds));
        return;
    }
    if (isOpen()) {
        ArmHardwareClient::RequestHardwareStatusOutgoingCommand *cmd =
                new ArmHardwareClient::RequestHardwareStatusOutgoingCommand(harwaresIds);
        connect(cmd, SIGNAL(finish(bool)), this, SLOT(onRequestStatusesCommandDone(bool)));
        mClient->sendOutgoingCommand(cmd);
    }
}

void ThreadWorker::setNotifyCurrentNotifyContext(const QStringList &hardwaresIds, const ArmHardwareClient::NotifyContext &context)
{
    QMutexLocker l(&mMutex);
    mHardwaresIdsForStartNotifyCommand = hardwaresIds;
    mContext = context;
}

void ThreadWorker::onIncomingCommand(ArmHardwareClient::IIncomingCommandSharedPtr command)
{
    switch (command->commandType()) {
    case ArmHardwareClient::IIncomingCommand::NotifyStatusChange:
        handleChangeNotifyStatusCommand(command);
        break;

    case ArmHardwareClient::IIncomingCommand::HardwareStatusChange:
        handleChangeHardwareStatusCommand(command);
        break;

    case ArmHardwareClient::IIncomingCommand::Message:
        handleMessageCommand(command);
        break;

    default:
        break;
    }
}

void ThreadWorker::onConnectedToServer()
{
    emit logMessage(tr("Соеденение с сервером %0:%1: успех")
                    .arg(mServerAddres, QString::number(mServerPort)));
    emit connectedToServer();
}

void ThreadWorker::onDisconnectedFromServer()
{
    emit logMessage(tr("Дисконнект от сервера %0:%1")
                    .arg(mServerAddres, QString::number(mServerPort)));
    close();
    emit disconnectedFromServer();
}

void ThreadWorker::onError()
{
    emit logMessage(tr("Соеденение с сервером %0:%1: ошибка")
                    .arg(mServerAddres, QString::number(mServerPort)));
    close();
    emit error();
}

void ThreadWorker::onRequestStatusesCommandDone(const bool success)
{
    ArmHardwareClient::RequestHardwareStatusOutgoingCommand *cmd = qobject_cast<ArmHardwareClient::RequestHardwareStatusOutgoingCommand*>(sender());
    if (success) {
        if (cmd) {
            QString message = tr("Получена квитанция на команду запроса статусов оборудования.");
            emit logMessage(message);
            auto statuses = cmd->hardwareStatuses();
            if (statuses.isEmpty()) {
                message = tr("Нет информации об оборудовании");
                emit logMessage(message);
            } else {
                for (const ArmHardwareClient::HardwareStatus &status : statuses) {
                    message = tr("Id: %0, состояние оборудования: %1 (%3), состояние оповещения: %2 (%4)")
                            .arg(status.hardwareId).arg((int)status.status).arg((int)status.notifyStatus);
                    message = message.arg(status.statusCheckTime.toString(), status.notifyStatusCheckTime.toString());
                    emit logMessage(message);
                }
            }
        } else {
            emit logMessage(tr("В процедуре обработки результата от запроса статусов оборудования"));
            emit logMessage(tr("не получилось привести результат к нужному типу"));
        }
    } else {
        emit logMessage(tr("Ошибка при посылке команды запроса статусов оборудования"));
    }
    emit logMessage("-----------------------------------");

    if (cmd) {
        cmd->deleteLater();
    }
}

void ThreadWorker::onStartNotifyCommandDone(const bool success)
{
    ArmHardwareClient::StartNotifyOutgoingCommand *cmd = qobject_cast<ArmHardwareClient::StartNotifyOutgoingCommand*>(sender());
    if(success) {
        if (cmd) {
            QStringList hardwares = cmd->acceptedIds();
            QString message = tr("Получена квитанция на команду запуска оповещения.");
            emit logMessage(message);
            if (hardwares.isEmpty()) {
                message = tr("Нет подтверждения о принятия команды ни одним оборудованием");
                emit logMessage(message);
            } else {
                message = tr("Подтверждённые идентификаторы оборудования:");
                emit logMessage(message);
                for (const QString &id : hardwares) {
                    emit logMessage(id);
                }
            }

        } else {
            emit logMessage(tr("В процедуре обработки результата от команды запуска оповещения"));
            emit logMessage(tr("не получилось привести результат к нужному типу"));
        }
    } else {
        emit logMessage(tr("Ошибка при посылке команды запуска оповещения"));
    }
    emit logMessage("-----------------------------------");
    if (cmd) {
        cmd->deleteLater();
    }
}

void ThreadWorker::handleChangeNotifyStatusCommand(ArmHardwareClient::IIncomingCommandSharedPtr command)
{
    using namespace ArmHardwareClient;
    ChangeNotifyStatysIncomingCommand* cmd = qobject_cast<ChangeNotifyStatysIncomingCommand*>(command.data());
    if (cmd) {
        QString message = tr("Входящее оповещение об изменении статуса оповещения с ИД '%0'").arg(cmd->hardwareId());
        emit logMessage(message);
        message = tr("Время изменения статуса: %0").arg(cmd->stateChangeTime().toString());
        emit logMessage(message);
        message = tr("Код статуса: %0").arg((int)cmd->notifyState());
        emit logMessage(message);
        emit logMessage("-------------------------------");
    }
}

void ThreadWorker::handleMessageCommand(ArmHardwareClient::IIncomingCommandSharedPtr command)
{
    using namespace ArmHardwareClient;
    MessageIncomingCommand* cmd = qobject_cast<MessageIncomingCommand*>(command.data());
    if (cmd) {
        QString message = tr("Входящее сообщение от оборудования с ИД '%0'").arg(cmd->hardwareId());
        emit logMessage(message);
        message = tr("Сообщение: %0").arg(cmd->message());
        emit logMessage(message);
        emit logMessage("-------------------------------");
    }
}

void ThreadWorker::handleChangeHardwareStatusCommand(ArmHardwareClient::IIncomingCommandSharedPtr command)
{
    using namespace ArmHardwareClient;
    ChangeHardwareStatusIncomingCommand* cmd = qobject_cast<ChangeHardwareStatusIncomingCommand*>(command.data());
    if (cmd) {
        QString message = tr("Входящее оповещение об изменении статуса оборудования с ИД '%0'").arg(cmd->hardwareId());
        emit logMessage(message);
        message = tr("Время изменения статуса: %0").arg(cmd->stateChangeTime().toString());
        emit logMessage(message);
        message = tr("Код статуса: %0").arg((int)cmd->hardwareState());
        emit logMessage(message);
        emit logMessage("-------------------------------");
    }

}
