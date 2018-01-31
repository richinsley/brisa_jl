#ifndef TVCONTROLPOINT_H
#define TVCONTROLPOINT_H

#include <QMainWindow>
#include <QSplashScreen>
#include "controlpoint/brisacontrolpoint.h"
#include <QListWidgetItem>
#define DEVICE_TYPE  "urn:schemas-upnp-org:device:TVDevice:1"
#define SERVICE_TYPE "urn:schemas-upnp-org:service:TVOperations:1"
#define DEVICE_UDN   "uuid:93ff89c4-261e-463d-b79e-d52cb241c171"

using namespace Brisa;

namespace Ui
{
    class TVControlPointClass;
}

class TVControlPoint : public QMainWindow
{
    Q_OBJECT

public:
    QSplashScreen *splash;

    TVControlPoint(QWidget *parent = 0);
    ~TVControlPoint();

signals:
    void timeStop();

private:
    Ui::TVControlPointClass *ui;
    BrisaControlPoint *tvControlPoint;
    int contSplashScreen;
    QList<BrisaControlPointDevice *> tvs;
    QStringList getActions(BrisaControlPointService *s);
    QStringList getServices(BrisaControlPointDevice *c);
    void addTV(BrisaControlPointDevice* tv);
    QListWidgetItem* removeFromList(QString udn);
    void restart();
    void callService(BrisaOutArgument result, QString operation);
    BrisaControlPointDevice* getDeviceByUDN(QString UDN);
    void _OffOn(BrisaControlPointDevice *c);
    void _ChangeChanel(BrisaControlPointDevice *c, QString chanell);

public slots:
    void onNewDevice(BrisaControlPointDevice *c);
    void onRemovedDevice(QString Udn);
    void onCallService(BrisaOutArgument result, QString operation);

private slots:
    void on_btn_power_clicked();
    void on_btnPrior_clicked();
    void on_btn_next_chanel_clicked();
    void processSplashScreen();

};

#endif // TVCONTROLPOINT_H
