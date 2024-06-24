#ifndef CREADER_H
#define CREADER_H

#include <QString>
#include <QObject>
#include <QSerialPort>

class Creader : public QObject
{
    Q_OBJECT
private:
    bool fchrg = false;
public:
    Creader(QString prt_name);
    QSerialPort *port;
    QString str;
    void setVolume(bool tf);
    void stopReading();
public slots:
    void start_reading();
    void ready_data();
signals:
    void Rdy_Package(QString str);

};

#endif // CREADER_H
