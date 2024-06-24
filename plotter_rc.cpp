#include "plotter_rc.h"
#include "ui_plotter_rc.h"
#include <QSerialPortInfo>
#include <QtCore>


Plotter_RC::Plotter_RC(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Plotter_RC)
{
    ui->setupUi(this);



    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }

}

Plotter_RC::~Plotter_RC()
{
    delete ui;
}


void Plotter_RC::on_pushButton_clicked()
{
    for(int i = 0; i < ui->comboBox->count(); i++)
    {
        ui->comboBox->removeItem(0);
    }
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }
}

void Plotter_RC::plotting(QString str)
{
    data.append(str);

    QStringList sl = data.split(u',', QString::SkipEmptyParts);

    QVector<double> yAxis;
    QVector<double> xAxis;
    int counter = 0;

    foreach(QString part, sl)
    {
        yAxis.append(part.toDouble() / 1024 * 5.0);
        xAxis.append(counter * 0.05);
        counter++;
    }

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot->legend->setVisible(false);
    ui->plot->yAxis->setLabel("");
    ui->plot->xAxis->setLabel("Time, Sec");
    ui->plot->xAxis->setRange(range_start, range_end + addTime);
    ui->plot->yAxis->setRange(0, voltRange);
    ui->plot->clearGraphs();
    ui->plot->addGraph();

    ui->plot->graph()->setPen(QPen(Qt::red));
    ui->plot->graph()->setName("fft_chA");
    ui->plot->graph(0)->addData(xAxis, yAxis);
    ui->plot->replot();
    ui->plot->graph(0)->rescaleAxes();
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void Plotter_RC::getState()
{
    bool tf = ui->checkBox->checkState();
    prt->setVolume(tf);
    if(tf){
        voltRange = 5;
        addTime = 200;
    }
    else {
        voltRange = 3.3;
        addTime = 0;
    }
    ui->stop_btn->setEnabled(true);
    ui->start_btn->setEnabled(false);
}


void Plotter_RC::on_open_btn_clicked()
{
    prt = new Creader(ui->comboBox->currentText());

    connect(ui->start_btn, SIGNAL(clicked()), this, SLOT(getState()));
    connect(ui->start_btn, SIGNAL(clicked()), prt, SLOT(start_reading()));
    connect(prt, SIGNAL(Rdy_Package(QString)), this, SLOT(plotting(QString)));

    ui->open_btn->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->start_btn->setEnabled(true);
}


void Plotter_RC::on_stop_btn_clicked()
{
    data.clear();
    ui->open_btn->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->start_btn->setEnabled(false);

    prt->stopReading();
    delete prt;
}

