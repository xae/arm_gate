#include "hardwareservertestappmainwindow.h"
#include "ui_hardwareservertestappmainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QUuid>

#define OK_HARDWARE_STATE_COMBOBOX_INDEX 0
#define ERROR_HARDWARE_STATE_COMBOBOX_INDEX 1

#define WAIT_NOTIFY_STATE_COMBOBOX_INDEX 0
#define PROCESS_NOTIFY_STATE_COMBOBOX_INDEX 1


HardwareServerTestAppMainWindow::HardwareServerTestAppMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HardwareServerTestAppMainWindow)
{
    ui->setupUi(this);
#if QT_VERSION >= 0x050000
    connect(ui->treeView, &QTreeView::pressed,
            this, &HardwareServerTestAppMainWindow::onTreeItemActivated);
    connect(ui->comboBoxHardwareState, (void(QComboBox::*)(int))&QComboBox::currentIndexChanged,
            this, &HardwareServerTestAppMainWindow::onHardwareStateComboboxIndexChanged);
    connect(ui->comboBoxNotifyState, (void(QComboBox::*)(int))&QComboBox::currentIndexChanged,
            this, &HardwareServerTestAppMainWindow::onNotifyStateComboboxIndexChanged);
    connect(ui->lineEditHardwareId, &QLineEdit::textChanged,
            this, &HardwareServerTestAppMainWindow::onUuidChanged);
    connect(ui->spinBoxNotifyDuration, (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
            this, &HardwareServerTestAppMainWindow::onDurationChanged);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            this, &HardwareServerTestAppMainWindow::onCustomContextMenuRequested);
#else
    connect(ui->treeView, SIGNAL(pressed(QModelIndex)),
            this, SLOT(onTreeItemActivated(QModelIndex)));
    connect(ui->comboBoxHardwareState, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onHardwareStateComboboxIndexChanged(int)));
    connect(ui->comboBoxNotifyState, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onNotifyStateComboboxIndexChanged(int)));
    connect(ui->lineEditHardwareId, SIGNAL(textEdited(QString)),
            this, SLOT(onUuidChanged(QString)));
    connect(ui->spinBoxNotifyDuration, SIGNAL(valueChanged(int)),
            this, SLOT(onDurationChanged(int)));
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(onCustomContextMenuRequested(QPoint)));
#endif
    connect(ui->pushButtonSendMessage, SIGNAL(clicked()), this, SLOT(onSendMessageButtonPushed()));
    connect(ui->pushButtonSendHardwareState, SIGNAL(clicked()), this, SLOT(onSendCurrentHardwareStatusButtonPushed()));
    connect(ui->pushButtonSendNotifyState, SIGNAL(clicked()), this, SLOT(onSendCurrentNotifyStatusButtonPushed()));

    connect(&mModel, SIGNAL(newHardwareSetting(HardwareServerTestApp::HardwareInfo)),
            this, SIGNAL(newHardwareSetting(HardwareServerTestApp::HardwareInfo)));

    ui->comboBoxHardwareState->addItems({tr("Работает"), tr("Не работает")});
    ui->comboBoxNotifyState->addItems({tr("Ожидание"), tr("Оповещение")});

    ui->groupBoxHardwareProperties->setEnabled(false);
    ui->treeView->header()->hide();
    ui->treeView->setModel(&mModel);
    ui->treeView->setIndentation(0);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

}

HardwareServerTestAppMainWindow::~HardwareServerTestAppMainWindow()
{
    delete ui;
}

void HardwareServerTestAppMainWindow::setHardwareState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState)
{
    mModel.setHardwareState(hardwareId, newState);
    auto info = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (info.hardwareId == hardwareId) {
        setCurrentHardwareStatus(newState);
    }
}

void HardwareServerTestAppMainWindow::setNotifyState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState)
{
    mModel.setNotifyState(hardwareId, newState);
    auto info = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (info.hardwareId == hardwareId) {
        setCurrentNotifyStatus(newState);
    }
}

void HardwareServerTestAppMainWindow::addHardware(const HardwareServerTestApp::HardwareInfo &hardwareInfo)
{
    mModel.addHardware(hardwareInfo);
    using namespace HardwareServerTestApp;
    HardwareInfo info = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (info.globalId == hardwareInfo.globalId) {
        fillCurrentHardwareInfo(hardwareInfo);
    }
}

void HardwareServerTestAppMainWindow::logMessage(const QString &message)
{
    static const QString logTemplate(R"(<html><head/><body><p><span style=" font-size:8.25pt; color:#ff0000;">%0:</span><span style=" font-size:8.25pt;"> %1</span></p></body></html>)");
    ui->textEditLog->append(logTemplate.arg(QDateTime::currentDateTime().time().toString(), message));

}

void HardwareServerTestAppMainWindow::onTreeItemActivated(const QModelIndex &index)
{
    auto info = mModel.hardwareInfo(index);
    if (info.hardwareId.isEmpty()) {
        ui->groupBoxHardwareProperties->setEnabled(false);
    } else {
        ui->groupBoxHardwareProperties->setEnabled(true);
        fillCurrentHardwareInfo(info);
    }
}

void HardwareServerTestAppMainWindow::onNotifyStateComboboxIndexChanged(const int index)
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (hi.globalId.isEmpty()) {
        return;
    }
    hi.notifyState = index == PROCESS_NOTIFY_STATE_COMBOBOX_INDEX ?
                HardwareInfo::Process : HardwareInfo::Wait;
    addHardware(hi);
    emit notifyStateChange(hi.globalId, hi.notifyState);
}

void HardwareServerTestAppMainWindow::onHardwareStateComboboxIndexChanged(const int index)
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (hi.globalId.isEmpty()) {
        return;
    }
    hi.hardwareState = index == OK_HARDWARE_STATE_COMBOBOX_INDEX ?
                HardwareInfo::Ok : HardwareInfo::Error;
    addHardware(hi);
    emit hardwareStateChange(hi.globalId, hi.hardwareState);
}

void HardwareServerTestAppMainWindow::onUuidChanged(const QString &uuid)
{
    using namespace HardwareServerTestApp;
   HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (hi.globalId.isEmpty()) {
        return;
    }
    hi.hardwareId = uuid;
    addHardware(hi);
    emit newHardwareSetting(hi);
}

void HardwareServerTestAppMainWindow::onDurationChanged(const int duration)
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (hi.globalId.isEmpty()) {
        return;
    }
    hi.notifyDuration = duration;
    addHardware(hi);
    emit newHardwareSetting(hi);
}

void HardwareServerTestAppMainWindow::onSendMessageButtonPushed()
{
    using namespace HardwareServerTestApp;
    QString msg = ui->lineEditMessage->text();
    if (!msg.isEmpty()) {
        HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
        if (hi.globalId.isEmpty()) {
            return;
        }
        emit message(hi.globalId, msg);
    }
}

void HardwareServerTestAppMainWindow::onSendCurrentNotifyStatusButtonPushed()
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (hi.globalId.isEmpty()) {
        return;
    }
    emit notifyStateChange(hi.globalId, hi.notifyState);
}

void HardwareServerTestAppMainWindow::onSendCurrentHardwareStatusButtonPushed()
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (hi.globalId.isEmpty()) {
        return;
    }
    emit hardwareStateChange(hi.globalId, hi.hardwareState);
}

void HardwareServerTestAppMainWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    auto index = ui->treeView->indexAt(pos);
    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToHide()),
            menu, SLOT(deleteLater()));
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(index);
    if (!hi.globalId.isEmpty()) {
        QAction *action = menu->addAction(tr("Удалить оборудование '%0'").arg(hi.description));

        connect(action, SIGNAL(triggered()),
                this, SLOT(deleteHardwareActionTriggered()));
        menu->addSeparator();
    }
    QAction *action = menu->addAction(tr("Добавить оборудование"));
    connect(action, SIGNAL(triggered()), this, SLOT(addHardwareActionTriggered()));
    menu->popup(QCursor::pos());
}

void HardwareServerTestAppMainWindow::deleteHardwareActionTriggered()
{
    using namespace HardwareServerTestApp;
    HardwareInfo hi = mModel.hardwareInfo(ui->treeView->currentIndex());
    if (!hi.globalId.isEmpty()) {
        mModel.deleteHardware(hi);
        emit removeHardware(hi);
    }
}

void HardwareServerTestAppMainWindow::addHardwareActionTriggered()
{
    auto genUuid = []() -> QString {
        return QString::fromLocal8Bit(QUuid::createUuid().toRfc4122().toHex());
    };
    using namespace HardwareServerTestApp;
    HardwareInfo hi;
    hi.description = QString("hardware %0").arg(genUuid().mid(0, 8));
    hi.globalId = genUuid();
    hi.hardwareId = genUuid();
    hi.notifyDuration = 10;
    hi.notifyState = HardwareInfo::Wait;
    hi.hardwareState = HardwareInfo::Ok;
    addHardware(hi);
    emit newHardwareSetting(hi);
}

void HardwareServerTestAppMainWindow::fillCurrentHardwareInfo(const HardwareServerTestApp::HardwareInfo &hardwareInfo)
{
    ui->lineEditHardwareId->setText(hardwareInfo.hardwareId);
    ui->spinBoxNotifyDuration->setValue(hardwareInfo.notifyDuration);
    setCurrentHardwareStatus(hardwareInfo.hardwareState);
    setCurrentNotifyStatus(hardwareInfo.notifyState);
}

void HardwareServerTestAppMainWindow::setCurrentNotifyStatus(const HardwareServerTestApp::HardwareInfo::NotifyState &state)
{
    int index = WAIT_NOTIFY_STATE_COMBOBOX_INDEX;
    if (state == HardwareServerTestApp::HardwareInfo::Process) {
        index = PROCESS_NOTIFY_STATE_COMBOBOX_INDEX;
    }
    ui->comboBoxNotifyState->blockSignals(true);
    ui->comboBoxNotifyState->setCurrentIndex(index);
    ui->comboBoxNotifyState->blockSignals(false);
}

void HardwareServerTestAppMainWindow::setCurrentHardwareStatus(const HardwareServerTestApp::HardwareInfo::HardwareState &state)
{
    int index = ERROR_HARDWARE_STATE_COMBOBOX_INDEX;
    if (state == HardwareServerTestApp::HardwareInfo::Ok) {
        index = OK_HARDWARE_STATE_COMBOBOX_INDEX;
    }
    ui->comboBoxHardwareState->blockSignals(true);
    ui->comboBoxHardwareState->setCurrentIndex(index);
    ui->comboBoxHardwareState->blockSignals(false);
}

Model::Model(QObject *parent) :
    QStandardItemModel(parent)
{
#if QT_VERSION >= 0x050000
    connect(this, &Model::itemChanged,
            this, &Model::onItemChanged);
#else
    connect(this, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(onItemChanged(QStandardItem*)));
#endif
}

Model::~Model()
{

}

void Model::addHardware(const HardwareServerTestApp::HardwareInfo &hardwareInfo)
{
    using namespace HardwareServerTestApp;
    for (int i = 0; i < rowCount(); ++i) {
        QStandardItem *item = this->item(i);
        if (item) {
            if (item->data().canConvert<HardwareServerTestApp::HardwareInfo>()) {
                HardwareInfo info = item->data().value<HardwareInfo>();
                if (info.globalId == hardwareInfo.globalId) {
                    blockSignals(true);
                    item->setData(QVariant::fromValue<HardwareInfo>(hardwareInfo));
                    item->setText(hardwareInfo.description);
                    blockSignals(false);
                    return;
                }
            }
        }
    }
    auto item = new QStandardItem(hardwareInfo.description);
    item->setData(QVariant::fromValue<HardwareInfo>(hardwareInfo));
    item->setText(hardwareInfo.description);
    appendRow(item);
}

void Model::deleteHardware(const HardwareServerTestApp::HardwareInfo &hardwareInfo)
{
    using namespace HardwareServerTestApp;
    for (int i = 0; i < rowCount(); ++i) {
        QStandardItem *item = this->item(i);
        if (item) {
            if (item->data().canConvert<HardwareServerTestApp::HardwareInfo>()) {
                HardwareInfo info = item->data().value<HardwareInfo>();
                if (info.globalId == hardwareInfo.globalId) {
                    removeRow(item->row());
                    return;
                }
            }
        }
    }
}

void Model::setHardwareState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::HardwareState &newState)
{
    using namespace HardwareServerTestApp;
    for (int i = 0; i < rowCount(); ++i) {
        QStandardItem *item = this->item(i);
        if (item) {
            if (item->data().canConvert<HardwareServerTestApp::HardwareInfo>()) {
                HardwareInfo info = item->data().value<HardwareInfo>();
                if (info.hardwareId == hardwareId) {
                    info.hardwareState = newState;
                    blockSignals(true);
                    item->setData(QVariant::fromValue<HardwareInfo>(info));
                    blockSignals(false);
                }
            }
        }
    }
}

void Model::setNotifyState(const QString &hardwareId, const HardwareServerTestApp::HardwareInfo::NotifyState &newState)
{
    using namespace HardwareServerTestApp;
    for (int i = 0; i < rowCount(); ++i) {
        QStandardItem *item = this->item(i);
        if (item) {
            if (item->data().canConvert<HardwareServerTestApp::HardwareInfo>()) {
                HardwareInfo info = item->data().value<HardwareInfo>();
                if (info.hardwareId == hardwareId) {
                    info.notifyState = newState;
                    blockSignals(true);
                    item->setData(QVariant::fromValue<HardwareInfo>(info));
                    blockSignals(false);
                }
            }
        }
    }
}

HardwareServerTestApp::HardwareInfo Model::hardwareInfo(const QModelIndex &index)
{
    HardwareServerTestApp::HardwareInfo retVal;
    QStandardItem *item = itemFromIndex(index);
    if (item && item->data().canConvert<HardwareServerTestApp::HardwareInfo>()) {
        retVal = item->data().value<HardwareServerTestApp::HardwareInfo>();
    }
    return retVal;
}

void Model::onItemChanged(QStandardItem *item)
{
    using namespace HardwareServerTestApp;
    if (item->data().canConvert<HardwareInfo>()) {
        HardwareInfo info = item->data().value<HardwareInfo>();
        info.description = item->text();
        blockSignals(true);
        item->setData(QVariant::fromValue<HardwareInfo>(info));
        blockSignals(false);
        emit newHardwareSetting(info);
    }
}
