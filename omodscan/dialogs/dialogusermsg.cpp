#include <QTimer>
#include <QPushButton>
#include <QMessageBox>
#include <QDialogButtonBox>
#include "modbuslimits.h"
#include "dialogusermsg.h"
#include "ui_dialogusermsg.h"

///
/// \brief DialogUserMsg::DialogUserMsg
/// \param slaveAddress
/// \param func
/// \param mode
/// \param client
/// \param parent
///
DialogUserMsg::DialogUserMsg(quint8 slaveAddress, QModbusPdu::FunctionCode func, DataDisplayMode mode, ModbusClient& client, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogUserMsg)
    ,_mm(nullptr)
    ,_modbusClient(client)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog |
                   Qt::CustomizeWindowHint |
                   Qt::WindowTitleHint);

    ui->lineEditSlaveAddress->setInputRange(ModbusLimits::slaveRange());
    ui->lineEditSlaveAddress->setValue(slaveAddress);
    ui->comboBoxFunction->addItems(ModbusFunction::validCodes());
    ui->comboBoxFunction->setCurrentFunctionCode(func);
    ui->responseInfo->setShowTimestamp(false);

    switch(mode)
    {
        case DataDisplayMode::Binary:
        case DataDisplayMode::Hex:
            ui->radioButtonHex->click();
        break;
        default:
            ui->radioButtonDecimal->click();
        break;
    }

    ui->sendData->setFocus();
    connect(&_modbusClient, &ModbusClient::modbusReply, this, &DialogUserMsg::on_modbusReply);
}

///
/// \brief DialogUserMsg::~DialogUserMsg
///
DialogUserMsg::~DialogUserMsg()
{
    delete ui;
    if(_mm) delete _mm;
}

///
/// \brief DialogUserMsg::changeEvent
/// \param event
///
void DialogUserMsg::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }

    QDialog::changeEvent(event);
}

///
/// \brief DialogUserMsg::on_pushButtonSend_clicked
///
void DialogUserMsg::on_pushButtonSend_clicked()
{
    ui->responseBuffer->clear();
    ui->responseInfo->clear();

    if(_modbusClient.state() != QModbusDevice::ConnectedState)
    {
        QMessageBox::warning(this, windowTitle(), tr("No connection to device"));
        return;
    }

    ui->pushButtonSend->setEnabled(false);

    QModbusRequest request;
    request.setFunctionCode(ui->comboBoxFunction->currentFunctionCode());
    request.setData(ui->sendData->value());

    _modbusClient.sendRawRequest(request, ui->lineEditSlaveAddress->value<int>(), 0);

    const auto timeout = _modbusClient.timeout() * _modbusClient.numberOfRetries();
    QTimer::singleShot(timeout, this, [&] { ui->pushButtonSend->setEnabled(true); });
}

///
/// \brief DialogUserMsg::on_modbusReply
/// \param reply
///
void DialogUserMsg::on_modbusReply(QModbusReply* reply)
{
    if(!reply) return;

    if(0 != reply->property("RequestId").toInt())
    {
        return;
    }

    if(reply->error() != QModbusDevice::NoError &&
        reply->error() != QModbusDevice::ProtocolError)
    {
        QMessageBox::warning(this, windowTitle(), reply->errorString());
        return;
    }

    if(_mm) delete _mm;
    const auto protocol = _modbusClient.connectionType() == ConnectionType::Tcp ? ModbusMessage::Tcp : ModbusMessage::Rtu;
    _mm = ModbusMessage::create(reply->rawResult(), protocol, reply->serverAddress(), QDateTime::currentDateTime(), false);

    ui->responseBuffer->setValue(*_mm);
    ui->responseInfo->setModbusMessage(_mm);

    ui->pushButtonSend->setEnabled(true);
}

///
/// \brief DialogUserMsg::on_radioButtonHex_clicked
/// \param checked
///
void DialogUserMsg::on_radioButtonHex_clicked(bool checked)
{
    if(checked)
    {
        ui->comboBoxFunction->setInputMode(FunctionCodeComboBox::HexMode);
        ui->sendData->setInputMode(ByteListTextEdit::HexMode);
        ui->responseBuffer->setInputMode(ByteListTextEdit::HexMode);
        ui->responseInfo->setDataDisplayMode(DataDisplayMode::Hex);
    }
}

///
/// \brief DialogUserMsg::on_radioButtonDecimal_clicked
/// \param checked
///
void DialogUserMsg::on_radioButtonDecimal_clicked(bool checked)
{
    if(checked)
    {
        ui->comboBoxFunction->setInputMode(FunctionCodeComboBox::DecMode);
        ui->sendData->setInputMode(ByteListTextEdit::DecMode);
        ui->responseBuffer->setInputMode(ByteListTextEdit::DecMode);
        ui->responseInfo->setDataDisplayMode(DataDisplayMode::Decimal);
    }
}
