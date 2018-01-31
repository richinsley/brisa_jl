#include "tvcontrolpoint.h"
#include "ui_tvcontrolpoint.h"

TVControlPoint::TVControlPoint(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TVControlPointClass),
    tvControlPoint(new BrisaControlPoint)
{
    contSplashScreen= 0;
    ui->setupUi(this);

    splash = new QSplashScreen(QPixmap("Figs/Splash.png"));

    connect(this->tvControlPoint,SIGNAL(deviceFound(BrisaControlPointDevice*)),this,SLOT(onNewDevice(BrisaControlPointDevice*)));
    connect(this->tvControlPoint,SIGNAL(deviceGone(QString)),this,SLOT(onRemovedDevice(QString)));

    tvControlPoint->start();
    tvControlPoint->discover();
}

TVControlPoint::~TVControlPoint()
{
    delete ui;
}

void TVControlPoint::restart(){
    tvControlPoint->stop();
    tvControlPoint->start();
    tvControlPoint->discover();
}




/**
  *method call when one new device is found on network
 **/
void TVControlPoint::onNewDevice(BrisaControlPointDevice *c){

    if(c->getAttribute(BrisaControlPointDevice::DeviceType).compare(DEVICE_TYPE))
        return;

    for(int i = 0; i < tvs.size(); i++) {
        if(tvs[i]->getAttribute(BrisaControlPointDevice::Udn) == c->getAttribute(BrisaControlPointDevice::Udn)) {
            tvs[i] = c;
            return;
        }
    }

    this->tvs.append(c);
    qDebug("found");
    this->addTV(c);
}


/**
  *add one new TVDevice in DeviceList
*/
void TVControlPoint::addTV(BrisaControlPointDevice* tv){
    QString item = tv->getAttribute(BrisaControlPointDevice::FriendlyName) + " - "+ tv->getAttribute(BrisaControlPointDevice::Udn);
    QListWidgetItem* device = new QListWidgetItem();
    device->setData(Qt::UserRole,QString(tv->getAttribute(BrisaControlPointDevice::Udn)));
    device->setText(item);
    qDebug() << device;
    this->ui->li_devices->addItem(device);
}

/**
  * Slot call when one device was removed.
 **/
//this slot remove device from device list if the device leave
void TVControlPoint::onRemovedDevice(QString udn){
    BrisaControlPointDevice * cd = getDeviceByUDN(udn);
    if(cd && tvs.contains(cd))
    {
        tvs.removeOne(cd);
        qDebug() << "item removed :" + removeFromList(udn)->text();
        qDebug() << udn;
        restart();
    }
}

QListWidgetItem* TVControlPoint::removeFromList(QString udn){
    qDebug() << "udn to remove:" + udn.split("::")[0];
    int i = 0;
    for(; i < ui->li_devices->count(); i++){
        QListWidgetItem* currentItem = ui->li_devices->item(i);
        QString currentUDN = currentItem->data(Qt::UserRole).toString().section(",",0,0);
        qDebug() << "current Device udn:" + currentUDN;
        if(currentUDN == udn.split("::")[0])
            break;
    }
    qDebug() << ui->li_devices->item(i);
    return ui->li_devices->takeItem(i);
}


//get result after did operation in device
void TVControlPoint::callService(BrisaOutArgument result, QString operacao){
    qDebug(qPrintable(operacao + " returned " + result.value(operacao)));
}

/**
  * get service of device
 **/
QStringList TVControlPoint::getServices(BrisaControlPointDevice *c){
    QStringList Services;

    foreach(BrisaControlPointService *s, c->getServiceList().toVector())
        Services.append(s->getAttribute(BrisaControlPointService::ServiceType));

    return Services;
}

/**
 *get actions of one service
**/
QStringList TVControlPoint::getActions(BrisaControlPointService *s){
    QStringList actions;
    foreach(BrisaAction *a, s->getActionList().toVector())
        actions.append(a->getName());
    return actions;
}

/**
  * get Udn of one device
 **/
BrisaControlPointDevice* TVControlPoint::getDeviceByUDN(QString UDN)
{
    for(int i = 0; i < tvs.size(); i++) {
        if(UDN.compare(tvs[i]->getAttribute(BrisaControlPointDevice::Udn)) == 0)
            return tvs[i];
    }
    return NULL;
}

/**
  * call action OffOn
 **/
void TVControlPoint::_OffOn(BrisaControlPointDevice *c)
{
    BrisaControlPointService *Service = c->getServiceByType(SERVICE_TYPE);
    connect(Service, SIGNAL(requestFinished(QString,QString)), this, SLOT(callrService(QString, QString)));
    QMap<QString, QString> atributes;
    Service->call("OffOn", atributes);
}


/**
  * call action ChangeChanel
 **/
void TVControlPoint::_ChangeChanel(BrisaControlPointDevice *c, QString chanel)
{
    BrisaControlPointService *Service = c->getServiceByType(SERVICE_TYPE);
    connect(Service, SIGNAL(requestFinished(BrisaOutArgument,QString)), this, SLOT(callrService(BrisaOutArgument, QString)));
    QMap<QString, QString> atributes;
    atributes["NewChanel"] = chanel;
    Service->call("SetChanel", atributes);
}

/**
  * get result after operation in device
 **/
void TVControlPoint::onCallService(BrisaOutArgument result, QString operacao)
{
    qDebug(qPrintable(operacao + " returned " + result.value(operacao)));
}

/**
  * Screen of Splash
 **/
void TVControlPoint::processSplashScreen()
{
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Configuring Brisa..."),topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Starting..."),topRight, Qt::blue);
        break;
    }

    if (contSplashScreen > 2) {
        this->show();
        splash->finish(this);
        emit timeStop();
        delete splash;
    }
    contSplashScreen++;
}

/**
  * Callback button btn_next_chanel
 **/
void TVControlPoint::on_btn_next_chanel_clicked()
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString udn = ui->li_devices->currentItem()->text().split(" - ")[1];
        BrisaControlPointDevice *c = getDeviceByUDN(udn);
        _ChangeChanel(c,"next" + QString::number(rand()));
    }
}

/**
  * Callback button btnPrior
 **/
void TVControlPoint::on_btnPrior_clicked()
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString udn = ui->li_devices->currentItem()->text().split(" - ")[1];
        BrisaControlPointDevice *c = getDeviceByUDN(udn);
        _ChangeChanel(c,"prior" + QString::number(rand()));
    }
}

/**
  * Callback button btn_off
 **/
void TVControlPoint::on_btn_power_clicked()
{
    if(ui->li_devices->selectedItems().count() == 1){
        QString udn = ui->li_devices->currentItem()->text().split(" - ")[1];
        BrisaControlPointDevice *c = getDeviceByUDN(udn);
        _OffOn(c);
    }
}
