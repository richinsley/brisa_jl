/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisadevicexmlhandlercp.cpp
 * Created:
 * Description: Implementation of BrisaDeviceParserContext, BrisaDeviceXMLHandlerCP and
 * BrisaServiceFetcher classes.
 * Authors: Name <email> @since 2009
 *
 *
 * Copyright (C) <2009> <Embbeded Systems and Pervasive Computing Laboratory>
 *
 * BRisa-Qt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "brisadevicexmlhandlercp.h"
#include "brisacontrolpointdevice.h"

#define PORT_INDEX 2

using namespace Brisa;

BrisaDeviceXMLHandlerCP::~BrisaDeviceXMLHandlerCP() {
}

void BrisaDeviceXMLHandlerCP::parseDevice(BrisaControlPointDevice *device,
        QTemporaryFile *tmp)
{
    QDomDocument document("Device");
    document.setContent(tmp);
    QDomElement element = document.documentElement();
    if (element.tagName() != "root")
        return;
    QDomNode n = element.firstChild();
    while (!n.isNull()) {
        element = n.toElement();
        if (element.tagName() == "specVersion") {
            QString major = element.elementsByTagName("major").at(0).toElement().text();
            QString minor = element.elementsByTagName("minor").at(0).toElement().text();
            if (major.isEmpty() || minor.isEmpty())
                return;
            device->setAttribute(BrisaControlPointDevice::Major, major);
            device->setAttribute(BrisaControlPointDevice::Minor, minor);
        } else if (element.tagName() == "device") {
            parseDevice(device, element);
        }
        n = n.nextSibling();
    }
}

void BrisaDeviceXMLHandlerCP::parseDevice(BrisaControlPointDevice *device, QDomElement &element)
{
    QString udn = element.elementsByTagName("UDN").at(0).toElement().text();

    //qDebug() << "\n<UDN_XML>\n\t" << udn <<  "\n</UDN_XML>\n ";

    QString friendlyName = element.elementsByTagName("friendlyName").at(0).toElement().text();
    QString deviceType = element.elementsByTagName("deviceType").at(0).toElement().text();
    QString manufacturer = element.elementsByTagName("manufacturer").at(0).toElement().text();
    QString manufacturerURL = element.elementsByTagName("manufacturerURL").at(0).toElement().text();
    QString modelName = element.elementsByTagName("manufacturer").at(0).toElement().text();
    QString modelNumber = element.elementsByTagName("modelNumber").at(0).toElement().text();
    QString modelURL = element.elementsByTagName("modelURL").at(0).toElement().text();
    QString serialNumber = element.elementsByTagName("serialNumber").at(0).toElement().text();
    device->setAttribute(BrisaControlPointDevice::Udn, udn);
    device->setAttribute(BrisaControlPointDevice::FriendlyName, friendlyName);
    device->setAttribute(BrisaControlPointDevice::DeviceType, deviceType);
    device->setAttribute(BrisaControlPointDevice::Manufacturer, manufacturer);
    device->setAttribute(BrisaControlPointDevice::ManufacturerUrl, manufacturerURL);
    device->setAttribute(BrisaControlPointDevice::ModelName, modelName);
    device->setAttribute(BrisaControlPointDevice::ModelNumber, modelNumber);
    device->setAttribute(BrisaControlPointDevice::ModelUrl, modelURL);
    device->setAttribute(BrisaControlPointDevice::SerialNumber, serialNumber);

    QDomNodeList serviceList = element.elementsByTagName("serviceList");
    if (serviceList.size() > 0) {
        QDomNodeList services = serviceList.at(0).toElement().elementsByTagName("service");
        int ssize = services.size();
        for (int i = 0; i < ssize; i++) {

            QDomElement serviceElement = services.at(i).toElement();
            BrisaControlPointService *service = parseService(serviceElement);

            validateURLBase(device);

            // jl - take this off the stack and into the heap
            BrisaServiceFetcher * f = new BrisaServiceFetcher(service, device->
                                  getAttribute(BrisaControlPointDevice::UrlBase)
                                  + service->getAttribute(BrisaControlPointService::ScpdUrl));

            if (!f->fetch()) {
                device->addService(service);
            } else {
                // TODO handle error
                qWarning() << "Failed to download service XML.";
            }
            delete f;
        }
    }

    QDomNodeList iconList = element.elementsByTagName("iconList");
    if (iconList.size() > 0) {
        QDomNodeList icons = iconList.at(0).toElement().elementsByTagName("icon");
        for (int i = 0; i < icons.size(); i++) {
            QDomElement iconElement = icons.at(0).toElement();
            BrisaIcon *icon = parseIcon(iconElement);
            device->addIcon(icon);
        }
    }

    QDomNodeList deviceList = element.firstChildElement("deviceList").childNodes();

    for (int i = 0; i < deviceList.size(); i++) {
        BrisaControlPointDevice *embDevice = new BrisaControlPointDevice(device);
        QDomElement deviceElement = deviceList.at(i).toElement();
        parseDevice(embDevice, deviceElement);
        if(!embDevice->getAttribute(BrisaControlPointDevice::FriendlyName).isEmpty())
            device->addDevice(embDevice);
    }
}

BrisaControlPointService *BrisaDeviceXMLHandlerCP::parseService(QDomElement &element)
{
    QString serviceType = element.elementsByTagName("serviceType").at(0).toElement().text();
    QString serviceId = element.elementsByTagName("serviceId").at(0).toElement().text();
    QString controlURL = element.elementsByTagName("controlURL").at(0).toElement().text();
    QString eventSubURL = element.elementsByTagName("eventSubURL").at(0).toElement().text();
    QString SCPDURL = element.elementsByTagName("SCPDURL").at(0).toElement().text();

    return new BrisaControlPointService(serviceType, serviceId, SCPDURL, controlURL, eventSubURL, "");
}

void BrisaDeviceXMLHandlerCP::validateURLBase(BrisaControlPointDevice *device)
{
    QStringList urlBase = device->getAttribute(BrisaControlPointDevice::UrlBase).split(":");
    if (urlBase.size() > PORT_INDEX) {
        QString port = urlBase[PORT_INDEX];
        QString newPort = "";
        quint8 index = 0;
        while (port[index].isDigit()) {
            newPort.append(port[index]);
            index++;
        }
        urlBase[PORT_INDEX] = newPort;
        while (urlBase.size() > PORT_INDEX + 1) urlBase.pop_back();
    }
    device->setAttribute(BrisaControlPointDevice::UrlBase, urlBase.join(":"));
}

BrisaIcon *BrisaDeviceXMLHandlerCP::parseIcon(QDomElement &element)
{
    QString mimetype = element.elementsByTagName("mimetype").at(0).toElement().text();
    QString width = element.elementsByTagName("width").at(0).toElement().text();
    QString height = element.elementsByTagName("height").at(0).toElement().text();
    QString depth = element.elementsByTagName("depth").at(0).toElement().text();
    QString url = element.elementsByTagName("url").at(0).toElement().text();
    return new BrisaIcon(mimetype, width, height, depth, url);
}





