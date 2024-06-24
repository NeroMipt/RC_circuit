#include "plotter_rc.h"
#include "ui_plotter_rc.h"

Plotter_RC::Plotter_RC(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Plotter_RC)
{
    ui->setupUi(this);
}

Plotter_RC::~Plotter_RC()
{
    delete ui;
}

