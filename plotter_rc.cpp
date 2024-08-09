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


    ui->lineEdit->setValidator( new QDoubleValidator(0, 100, -1, this) );
    ui->lineEdit_2->setValidator( new QDoubleValidator(0, 100, -1, this) );

    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot->legend->setVisible(false);
    ui->plot->yAxis->setLabel("");
    ui->plot->xAxis->setLabel("Time, Sec");
    ui->plot->xAxis->setRange(range_start, range_end + addTime);
    ui->plot->yAxis->setRange(0, voltRange);
    ui->plot->clearGraphs();

    ui->plot->addGraph();


    ui->plot->graph(0)->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setName("Theor");

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeOffset(int)));

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
    ui->plot->addGraph();

    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setName("Pract");
    ui->plot->graph(1)->setData(xAxis, yAxis);
    ui->plot->replot();
    ui->plot->graph(1)->rescaleAxes();
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void Plotter_RC::getState()
{
    data.clear();
    tf = ui->checkBox->checkState();
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

void Plotter_RC::changeOffset(int offset)
{
    QList< QCPGraph * > list;
    list = ui->plot->selectedGraphs();

    for(int i=0; i < list.size(); i++)
    {
        QVector<double> yAxis;
        QVector<double> xAxis;
        QCPGraphDataContainer::const_iterator begin = list[i]->data()->begin(); // get range begin iterator from index
        QCPGraphDataContainer::const_iterator end = list[i]->data()->end(); // get range end iterator from index
        double os = offset - begin->key;
        for (QCPGraphDataContainer::const_iterator it=begin; it!=end; ++it)
        {
            // iterator "it" will go through all selected data points, as an example, we calculate the value average
            xAxis.append(it->key + os);
            yAxis.append(it->value);
        }
        list[i]->setData(xAxis, yAxis);
    }
    ui->plot->replot();
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
    ui->start_btn->setEnabled(true);

    prt->chgRd();
}


void Plotter_RC::on_checkBox_2_stateChanged(int arg1)
{
    bool ttf = ui->checkBox->checkState();
    QString Rst = ui->lineEdit->text();
    QString Cst = ui->lineEdit_2->text();
    Rst.replace(",", ".");
    Cst.replace(",", ".");
    if(Rst.isEmpty() || Cst.isEmpty())
    {
        QMessageBox msgWarning;
        msgWarning.setText("WARNING!\nВведите значения сопротивления или емкости.");
        msgWarning.setIcon(QMessageBox::Warning);
        msgWarning.setWindowTitle("Caution");
        msgWarning.exec();
        ui->checkBox_2->setCheckState(Qt::Unchecked);
        return;
    }
    if(arg1 == 2)
    {
        double R = Rst.toDouble();
        double C = Cst.toDouble();
        double RC = R * C;
        double limit1 = 0;
        QVector<double> yA;
        QVector<double> xA;
        if(ttf) limit1 = -1 * qLn(0.07) * RC;
        else limit1 = RC;
        double i = 0.0;
        while(i <= limit1)
        {
            yA.append(5.0*(1 - qExp(-i/RC)));
            xA.append(i);
            i+=0.05;
        }
        i = 0.0;
        double lV = yA[yA.size() - 1];
        while(i + limit1 <= range_end + addTime)
        {
            yA.append(lV*qExp(-i/RC));
            xA.append(i + limit1);
            i+=0.05;
        }
        ui->plot->graph(0)->setData(xA, yA);
        ui->plot->graph(0)->setVisible(true);
        ui->plot->replot();

    }else
    {
        ui->plot->graph(0)->setVisible(false);
        ui->plot->replot();
    }
}

