#include "controlpointgui.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    controlpointgui window;

    QTimer *timer = new QTimer();

    QObject::connect(timer, SIGNAL(timeout()),&window,SLOT(processSplashScreen()));
    QObject::connect(&window, SIGNAL(timeStop()),timer,SLOT(stop()));
    QObject::connect(&window, SIGNAL(timeStop()),window.splash,SLOT(close()));

    window.splash->show();// Show the splash Screen
    timer->start(750);//Starts the timer with intervals of 500 miliseconds

    return a.exec();

}
