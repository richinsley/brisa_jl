#include <QApplication>
#include "tvcontrolpoint.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TVControlPoint w;

    QTimer *timer = new QTimer();

    QObject::connect(timer, SIGNAL(timeout()),&w,SLOT(processSplashScreen()));
    QObject::connect(&w, SIGNAL(timeStop()),timer,SLOT(stop()));
    QObject::connect(&w, SIGNAL(timeStop()),w.splash,SLOT(close()));

    w.splash->show();
    timer->start(750);

    return a.exec();
}
