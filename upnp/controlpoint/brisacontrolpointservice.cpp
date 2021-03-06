/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisaabstractservice.h
 * Created:
 * Description: This file defines the BrisaAbstractService class.
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

#include "brisacontrolpointservice.h"
#include "brisaservicexmlhandler.h"

#include <QtDebug>
#include <QIODevice>

using namespace Brisa;

BrisaControlPointService::BrisaControlPointService(QObject *parent) :
    BrisaAbstractService(parent) {
    connect(http, SIGNAL(responseReady()), this, SLOT(getResponse()));
}

BrisaControlPointService::BrisaControlPointService(const QString &serviceType,
        const QString &serviceId, const QString &scpdUrl,
        const QString &controlUrl, const QString &eventSubUrl,
        const QString &host, const QString &vendorDomainName, QObject *parent) :
    BrisaAbstractService(serviceType, serviceId, scpdUrl, controlUrl,
            eventSubUrl, host, vendorDomainName, parent) {
    connect(http, SIGNAL(responseReady()), this, SLOT(getResponse()));
}

BrisaControlPointService::BrisaControlPointService(
        BrisaControlPointService &serv) :
    BrisaAbstractService(NULL),
    lastMethod(serv.lastMethod)
{
    connect(http, SIGNAL(responseReady()), this, SLOT(getResponse()));
}

void BrisaControlPointService::parseFromXml(QTemporaryFile *xml) {
    BrisaServiceXMLHandler handler;
    handler.parseService(this, xml);
    xml->deleteLater();
}

void BrisaControlPointService::call(const QString &method, BrisaInArgument &param) {
    QtSoapMessage request;

    http->setAction("\"" + serviceType + "#" + method + "\"");

    request.setMethod(method, serviceType);

    foreach(QString s, param.keys())
        {
            request.addMethodArgument(s, "", param.value(s));
        }

    lastMethod = method;
    this->http->submitRequest(request, this->controlUrl);
}

void BrisaControlPointService::getResponse() {
    const QtSoapMessage &message = http->getResponse();

    if (message.isFault()) {
        emit requestError("Error: " + message.faultString().toString(),
                lastMethod);
        return;
    }

    BrisaOutArgument returnMessage;
    QList<BrisaArgument*> arguments =
            this->getAction(this->lastMethod)->getArgumentList();
    foreach (BrisaArgument * arg, arguments)
        {
            if (arg->getAttribute(BrisaArgument::Direction) == "out") {
                QString argName = arg->getAttribute(BrisaArgument::ArgumentName);
                returnMessage.insert(argName, message.method()[argName].toString());
            }
        }
    emit requestFinished(returnMessage, lastMethod);
}

