#ifndef CONNECTIONDETAILS_H
#define CONNECTIONDETAILS_H

#include <QHostAddress>
#include <QSerialPort>
#include <QModbusDevice>
#include <QDataStream>
#include <QSettings>
#include "enums.h"

///
/// \brief The TcpConnectionParams class
///
struct TcpConnectionParams
{
    quint16 ServicePort = 502;
    QString IPAddress = "127.0.0.1";

    void normalize()
    {
        IPAddress = IPAddress.isEmpty() ? "127.0.0.1" : IPAddress;
        ServicePort = qMax<quint16>(1, ServicePort);
    }
};
Q_DECLARE_METATYPE(TcpConnectionParams)

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QDataStream& operator <<(QDataStream& out, const TcpConnectionParams& params)
{
    out << params.IPAddress;
    out << params.ServicePort;

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QDataStream& operator >>(QDataStream& in, TcpConnectionParams& params)
{
    in >> params.IPAddress;
    in >> params.ServicePort;

    params.normalize();
    return in;
}

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QSettings& operator <<(QSettings& out, const TcpConnectionParams& params)
{
    out.setValue("TcpParams/IPAddress",     params.IPAddress);
    out.setValue("TcpParams/ServicePort",   params.ServicePort);

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QSettings& operator >>(QSettings& in, TcpConnectionParams& params)
{
    params.IPAddress    = in.value("TcpParams/IPAddress").toString();
    params.ServicePort  = in.value("TcpParams/ServicePort").toUInt();

    params.normalize();
    return in;
}

///
/// \brief The SerialConnectionParams class
///
struct SerialConnectionParams
{
    QString PortName;
    QSerialPort::BaudRate BaudRate = QSerialPort::Baud9600;
    QSerialPort::DataBits WordLength = QSerialPort::Data8;
    QSerialPort::Parity Parity = QSerialPort::NoParity;
    QSerialPort::StopBits StopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl FlowControl = QSerialPort::NoFlowControl;
    bool SetDTR = true;
    bool SetRTS = true;

    void normalize()
    {
        BaudRate = qBound(QSerialPort::Baud1200, BaudRate, QSerialPort::Baud115200);
        WordLength = qBound(QSerialPort::Data5, WordLength, QSerialPort::Data8);
        Parity = qBound(QSerialPort::NoParity, Parity, QSerialPort::MarkParity);
        FlowControl = qBound(QSerialPort::NoFlowControl, FlowControl, QSerialPort::SoftwareControl);
    }
};
Q_DECLARE_METATYPE(SerialConnectionParams)

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QDataStream& operator <<(QDataStream& out, const SerialConnectionParams& params)
{
    out << params.PortName;
    out << params.BaudRate;
    out << params.WordLength;
    out << params.Parity;
    out << params.StopBits;
    out << params.FlowControl;
    out << params.SetDTR;
    out << params.SetRTS;

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QDataStream& operator >>(QDataStream& in, SerialConnectionParams& params)
{
    in >> params.PortName;
    in >> params.BaudRate;
    in >> params.WordLength;
    in >> params.Parity;
    in >> params.StopBits;
    in >> params.FlowControl;
    in >> params.SetDTR;
    in >> params.SetRTS;

    params.normalize();
    return in;
}

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QSettings& operator <<(QSettings& out, const SerialConnectionParams& params)
{
    out.setValue("SerialParams/PortName",       params.PortName);
    out.setValue("SerialParams/BaudRate",       params.BaudRate);
    out.setValue("SerialParams/WordLength",     params.WordLength);
    out.setValue("SerialParams/Parity",         params.Parity);
    out.setValue("SerialParams/FlowControl",    params.FlowControl);
    out.setValue("SerialParams/DTR",            params.SetDTR);
    out.setValue("SerialParams/RTS",            params.SetRTS);

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QSettings& operator >>(QSettings& in, SerialConnectionParams& params)
{
    params.PortName    = in.value("SerialParams/PortName").toString();
    params.BaudRate    = (QSerialPort::BaudRate)in.value("SerialParams/BaudRate").toUInt();
    params.WordLength  = (QSerialPort::DataBits)in.value("SerialParams/WordLength").toUInt();
    params.Parity      = (QSerialPort::Parity)in.value("SerialParams/Parity").toUInt();
    params.FlowControl = (QSerialPort::FlowControl)in.value("SerialParams/FlowControl").toUInt();
    params.SetDTR      = in.value("SerialParams/DTR").toBool();
    params.SetRTS      = in.value("SerialParams/RTS").toBool();

    params.normalize();
    return in;
}

///
/// \brief The ModbusProtocolSelections class
///
struct ModbusProtocolSelections
{
    TransmissionMode Mode = TransmissionMode::RTU;
    quint32 SlaveResponseTimeOut = 250;
    quint32 NumberOfRetries = 3;
    quint32 InterFrameDelay = 0;
    bool ForceModbus15And16Func = false;

    void normalize()
    {
        Mode = qBound(TransmissionMode::ASCII, Mode, TransmissionMode::RTU);
        SlaveResponseTimeOut = qBound(10U, SlaveResponseTimeOut, 300000U);
        NumberOfRetries = qBound(1U, NumberOfRetries, 10U);
        InterFrameDelay = qBound(0U, InterFrameDelay, 300000U);
    }
};
Q_DECLARE_METATYPE(ModbusProtocolSelections)

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QDataStream& operator <<(QDataStream& out, const ModbusProtocolSelections& params)
{
    out << params.Mode;
    out << params.SlaveResponseTimeOut;
    out << params.NumberOfRetries;
    out << params.InterFrameDelay;
    out << params.ForceModbus15And16Func;

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QDataStream& operator >>(QDataStream& in, ModbusProtocolSelections& params)
{
    in >> params.Mode;
    in >> params.SlaveResponseTimeOut;
    in >> params.NumberOfRetries;
    in >> params.InterFrameDelay;
    in >> params.ForceModbus15And16Func;

    params.normalize();
    return in;
}

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QSettings& operator <<(QSettings& out, const ModbusProtocolSelections& params)
{
    out.setValue("ModbusParams/Mode",                   (uint)params.Mode);
    out.setValue("ModbusParams/SlaveResponseTimeOut",   params.SlaveResponseTimeOut);
    out.setValue("ModbusParams/NumberOfRetries",        params.NumberOfRetries);
    out.setValue("ModbusParams/InterFrameDelay",        params.InterFrameDelay);
    out.setValue("ModbusParams/ForceModbus15And16Func", params.ForceModbus15And16Func);

    return out;

}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QSettings& operator >>(QSettings& in, ModbusProtocolSelections& params)
{
    params.Mode                    = (TransmissionMode)in.value("ModbusParams/Mode").toUInt();
    params.SlaveResponseTimeOut    = in.value("ModbusParams/SlaveResponseTimeOut").toUInt();
    params.NumberOfRetries         = in.value("ModbusParams/NumberOfRetries").toUInt();
    params.InterFrameDelay         = in.value("ModbusParams/InterFrameDelay").toUInt();
    params.ForceModbus15And16Func  = in.value("ModbusParams/ForceModbus15And16Func").toBool();

    params.normalize();
    return in;
}

///
/// \brief The ConnectionDetails class
///
struct ConnectionDetails
{
    ConnectionType Type = ConnectionType::Tcp;
    TcpConnectionParams TcpParams;
    SerialConnectionParams SerialParams;
    ModbusProtocolSelections ModbusParams;
};
Q_DECLARE_METATYPE(ConnectionDetails)

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QDataStream& operator <<(QDataStream& out, const ConnectionDetails& params)
{
    out << params.Type;
    out << params.TcpParams;
    out << params.SerialParams;
    out << params.ModbusParams;

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QDataStream& operator >>(QDataStream& in, ConnectionDetails& params)
{
    in >> params.Type;
    in >> params.TcpParams;
    in >> params.SerialParams;
    in >> params.ModbusParams;

    return in;
}

///
/// \brief operator <<
/// \param out
/// \param params
/// \return
///
inline QSettings& operator <<(QSettings& out, const ConnectionDetails& params)
{
    out.setValue("ConnectionParams/Type", (uint)params.Type);
    out << params.TcpParams;
    out << params.SerialParams;
    out << params.ModbusParams;

    return out;
}

///
/// \brief operator >>
/// \param in
/// \param params
/// \return
///
inline QSettings& operator >>(QSettings& in, ConnectionDetails& params)
{
    params.Type = (ConnectionType)in.value("ConnectionParams/Type").toUInt();
    in >> params.TcpParams;
    in >> params.SerialParams;
    in >> params.ModbusParams;

    return in;
}

#endif // CONNECTIONDETAILS_H
