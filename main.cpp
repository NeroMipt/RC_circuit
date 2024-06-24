#include "plotter_rc.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Plotter_RC w;
    w.show();
    return a.exec();
}
