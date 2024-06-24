#ifndef PLOTTER_RC_H
#define PLOTTER_RC_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Plotter_RC; }
QT_END_NAMESPACE

class Plotter_RC : public QMainWindow
{
    Q_OBJECT

public:
    Plotter_RC(QWidget *parent = nullptr);
    ~Plotter_RC();

private:
    Ui::Plotter_RC *ui;
};
#endif // PLOTTER_RC_H
