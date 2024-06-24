#include "creader.h"
#include <QSerialPort>
#include <QtCore>



Creader::Creader(QString prt_name)
{
    port = new QSerialPort;
    port->setPortName(prt_name);
    port->setBaudRate(QSerialPort::Baud9600);
    bool is_open = port->open(QIODevice::ReadWrite);
    if (!is_open) {
        qDebug() << "Failed to open port: " << prt_name << ", error: " << port->errorString();
    } else {
        qDebug() << "Connected to port";
    }
    connect(port, SIGNAL(readyRead()), this, SLOT(ready_data()));
}

void Creader::setVolume(bool tf)
{
    fchrg = tf;
}

void Creader::stopReading()
{
    disconnect(port, SIGNAL(readyRead()), this, SLOT(ready_data()));
    port->close();
}

void Creader::start_reading()
{
    char *rdy_msg;
    if(fchrg)
        rdy_msg = "1";
    else rdy_msg = "0";
    port->write(rdy_msg, 4);
}

void Creader::ready_data()
{
    QByteArray arr;
    arr = port->readAll();
    str.append(arr);
    if(str.size() > 100 && str[str.size() - 1] == ",")
    {
        emit Rdy_Package(str);
        str.clear();
    }
}
