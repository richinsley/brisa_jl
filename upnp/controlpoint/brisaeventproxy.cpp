/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisaeventproxy.cpp
 * Created:
 * Description: Implementation of BrisaEventProxy class.
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
#include "brisaeventproxy.h"

#include <brisawebserversession.h>

using namespace Brisa;

BrisaEventProxy::BrisaEventProxy(const QStringList &callbackUrls,
                                 BrisaWebserver *webserver,
                                 int &deliveryPath,
                                 QString host,
                                 int port,
                                 QNetworkAccessManager *http,
                                 QString eventSub,
                                 QObject *parent) :
    BrisaAbstractEventSubscription(QString(), callbackUrls, -1, parent),
    requestId(-1),
    deliveryPath(deliveryPath),
    host(host),
    port(port),
    httpm(http),
    eventSub(eventSub),
    webServer(webserver)
{
}

BrisaEventProxy::~BrisaEventProxy() {
}

int BrisaEventProxy::getId() {
    return this->requestId;
}

void BrisaEventProxy::renew(const int &newTimeout) {
    // jl
    HHttpRequestHeader *renewReq = getRenewRequest(newTimeout);
    QUrl url;
    url.setHost(host);
    url.setPort(port);
    url.setScheme("http");
    QNetworkRequest req(url);
    QString mverb = renewReq->method() + " " + renewReq->path();
    QStringList keys = renewReq->keys();
    foreach (QString k, keys) {
        req.setRawHeader(k.toUtf8(), renewReq->value(k).toUtf8());
    }
    requestId = (qulonglong)httpm->sendCustomRequest(req, mverb.toUtf8());
    qDebug() << requestId << " renew: " << renewReq->toString();
    delete renewReq;
}

void BrisaEventProxy::subscribe(const int timeout) {
    // jl
    HHttpRequestHeader *subscribeReq = getSubscriptionRequest(timeout);
    QUrl url;
    url.setHost(host);
    url.setPort(port);
    url.setScheme("http");
    QNetworkRequest req(url);
    QString path = subscribeReq->path();
    if(path.isEmpty())
    {
        qDebug() << "subscription path empty";
    }
    QString mverb = subscribeReq->method() + " " + path;
    QStringList keys = subscribeReq->keys();
    foreach (QString k, keys) {
        req.setRawHeader(k.toUtf8(), subscribeReq->value(k).toUtf8());
    }
    requestId = (qulonglong)httpm->sendCustomRequest(req, mverb.toUtf8());
    qDebug() << requestId << " subscription: " << requestId << " : " << subscribeReq->toString();
    delete subscribeReq;
}

void BrisaEventProxy::unsubscribe(void) {
    // jl
    HHttpRequestHeader *unsubscribeReq = getUnsubscriptionRequest();
    QUrl url;
    url.setHost(host);
    url.setPort(port);
    url.setScheme("http");
    QNetworkRequest req(url);
    QString mverb = unsubscribeReq->method() + " " + unsubscribeReq->path();
    QStringList keys = unsubscribeReq->keys();
    foreach (QString k, keys) {
        req.setRawHeader(k.toUtf8(), unsubscribeReq->value(k).toUtf8());
    }
    qulonglong urequestId = (qulonglong)httpm->sendCustomRequest(req, mverb.toUtf8());
    qDebug() << urequestId << " unsubscribe: " << unsubscribeReq->toString();
    delete unsubscribeReq;
}

HHttpRequestHeader *BrisaEventProxy::getSubscriptionRequest(const int timeout) {
    HHttpRequestHeader *request = new HHttpRequestHeader("SUBSCRIBE", eventSub);

    // Remote host
    request->setValue("HOST", host + ":" + QString().setNum(port));

    // Our URL for receiving notifications
    const QUrl url = this->getUrl();

    qDebug() << "Url: " << url.host() << " port: " << url.port();
    request->setValue("CALLBACK", "<http://"
                                    + url.host()
                                    + ":"
                                    + QString().setNum(url.port())
                                    + "/"
                                    + QString().setNum(deliveryPath)
                                    + ">");

    request->setValue("NT", "upnp:event");
    request->setValue("TIMEOUT", (timeout > 0)
                                    ? "Second-" + QString().setNum(timeout)
                                    : "INFINITE"); // INFINITE is obsolete in UPnP 1.1
    return request;
}

HHttpRequestHeader *BrisaEventProxy::getRenewRequest(const int timeout) const {
    if (this->getSid().isEmpty()) {
        qWarning() << "Renew failed: SID field not filled.";
        return NULL;
    }

    HHttpRequestHeader *request = new HHttpRequestHeader("SUBSCRIBE", eventSub);
    request->setValue("HOST", host + ":" + port);
    request->setValue("SID", getSid());
    request->setValue("TIMEOUT", (timeout > 0)
                                    ? "Second-" + QString().setNum(timeout)
                                    : "INFINITE");
    return request;
}

HHttpRequestHeader *BrisaEventProxy::getUnsubscriptionRequest() const {
    HHttpRequestHeader *request = new HHttpRequestHeader("UNSUBSCRIBE", eventSub);
    request->setValue("HOST", this->host + ":" + this->port);
    request->setValue("SID", this->SID);
    return request;
}

void BrisaEventProxy::onRequest(const HttpRequest &request, BrisaWebserverSession *session)
{
    QByteArray sid = request.header("SID");

    if (sid.isEmpty()) {
        // TODO report subscription error to user and should close the connection too
        qDebug() << "SID header not present on event subscription response.";

        return;
    }

    QDomDocument doc;
    QMap<QString, QString> eventingVariables;

    doc.setContent(request.entityBody(), false);
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName("e:property");

    for (int i = 0; i < list.count(); i++) {
        QDomNodeList internal = list.at(i).childNodes();

        for (int j = 0; j < internal.count(); j++) {
            QDomElement stateVariable = internal.at(j).toElement();
            eventingVariables[stateVariable.tagName()] = stateVariable.text();
        }
    }

    emit eventNotification(this, eventingVariables);

    HttpResponse response(request.httpVersion(), HttpResponse::OK, true);
    response.setHeader("CONNECTION", "close");
    response.setHeader("CONTENT-LENGTH", "0");
    session->respond(response);
}

void BrisaEventProxy::setSid(QString &sid) {
    this->SID = sid;
}


