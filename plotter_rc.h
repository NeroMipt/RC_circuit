#ifndef PLOTTER_RC_H
#define PLOTTER_RC_H

#include <QMainWindow>
#include <creader.h>

#define range_start 0
#define range_end 80

QT_BEGIN_NAMESPACE
namespace Ui { class Plotter_RC; }
QT_END_NAMESPACE

class Plotter_RC : public QMainWindow
{
    Q_OBJECT

public:
    Plotter_RC(QWidget *parent = nullptr);
    ~Plotter_RC();

private slots:
    void on_pushButton_clicked();
    void plotting(QString str);
    void getState();
    void on_open_btn_clicked();

    void on_stop_btn_clicked();

private:
    Ui::Plotter_RC *ui;
    Creader *prt;
    QString data;
    double voltRange = 3.3;
    int addTime = 0;
};
#endif // PLOTTER_RC_H
