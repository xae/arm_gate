#include "hardwareclienttestmainwindow.h"
#include "ui_hardwareclienttestmainwindow.h"

#include <QUuid>
#include <QDateTime>

#include <algorithm>

HardwareClientTestMainWindow::HardwareClientTestMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HardwareClientTestMainWindow)
{
    ui->setupUi(this);
    ui->treeView->header()->hide();
    ui->treeView->setModel(&mModel);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(ui->pushButtonAddId, SIGNAL(clicked()), this, SLOT(addHardwareIdButtonPushed()));
    connect(ui->pushButtonDeleteId, SIGNAL(clicked()), this, SLOT(removeHardwareIdButtonPushed()));
    connect(ui->pushButtonConnectDisconnect, SIGNAL(clicked()), this, SLOT(connectDisconnectButtonPushed()));
    connect(ui->pushButtonRequestHardwareStatuses, SIGNAL(clicked()), this, SLOT(requestHardwareStatusesButtonPushed()));
    connect(ui->pushButtonStartNotify, SIGNAL(clicked()), this, SLOT(startNotifyButtonPushed()));

}

HardwareClientTestMainWindow::~HardwareClientTestMainWindow()
{
    delete ui;
}

ArmHardwareClient::NotifyContext HardwareClientTestMainWindow::notifyContext() const
{
    ArmHardwareClient::NotifyContext retVal;
    retVal.description = tr("Описание оповещения");
    retVal.emailBody = ui->lineEditEmailBody->text();
    retVal.emailSubject = ui->lineEditEmailSubject->text();
    retVal.multimediaFileHash = ui->lineEditMultimediaFileHash->text();
    retVal.multimediaFileName = ui->lineEditMultimediaFileName->text();
    retVal.multimediaLocalFilePath = ui->lineEditMultimediaLocalPath->text();
    retVal.notifyCode = ui->lineEditNotifyCode->text();
    retVal.notifyId = ui->lineEditNotifyId->text();
    retVal.smsText = ui->lineEditSmsText->text();
    retVal.synthesisText = ui->lineEditSynthText->text();
    retVal.synthesisTextMediaFileHash = ui->lineEditSynthFileHash->text();
    retVal.synthesisTextMediaFileName = ui->lineEditSynthFileName->text();
    retVal.synthesisTextMediaLocalFilePath = ui->lineEditSynthLocalPath->text();
    return retVal;
}

QStringList HardwareClientTestMainWindow::hardwaresIds() const
{
    QStringList retVal;
    for (int i = 0; i < mModel.rowCount(); ++i) {
        QStandardItem *item = mModel.item(i);
        if (item) {
            retVal.append(item->text());
        }
    }
    return retVal;
}

quint16 HardwareClientTestMainWindow::serverPort() const
{
    return ui->spinBoxPort->value();
}

QString HardwareClientTestMainWindow::serverAddress() const
{
    return ui->lineEditAddress->text();
}

void HardwareClientTestMainWindow::setConnectDisconnectButtonText(const QString &text)
{
    ui->pushButtonConnectDisconnect->setText(text);
    ui->pushButtonConnectDisconnect->setEnabled(true);
}

void HardwareClientTestMainWindow::addHardwareIdButtonPushed()
{
    mModel.appendRow(new QStandardItem(QString::fromLocal8Bit(QUuid::createUuid().toRfc4122().toHex()).left(8)));
}

void HardwareClientTestMainWindow::removeHardwareIdButtonPushed()
{
    auto toDelete = ui->treeView->selectionModel()->selectedRows();
    std::sort(toDelete.begin(), toDelete.end(), [](const QModelIndex &f, const QModelIndex &s) {
       return f.row() > s.row();
    });
    for (const QModelIndex &index : toDelete) {
        mModel.removeRow(index.row());
    }
}

void HardwareClientTestMainWindow::startNotifyButtonPushed()
{
    emit startNotify();
}

void HardwareClientTestMainWindow::requestHardwareStatusesButtonPushed()
{
    emit requestHardwareStatuses();
}

void HardwareClientTestMainWindow::connectDisconnectButtonPushed()
{
    ui->pushButtonConnectDisconnect->setEnabled(false);
    emit connectOrDisconnectRequested();
}

void HardwareClientTestMainWindow::logMessage(const QString &message)
{
    ui->plainTextEdit->appendPlainText(tr("%0: %1").arg(QTime::currentTime().toString(), message));
}
