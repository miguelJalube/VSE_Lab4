#include "processingclient.h"

#include <iostream>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <qlogging.h>
#include <QEventLoop>

RestCompute::RestCompute(uint16_t a, uint16_t b, uint16_t c, MathComputerProxyClient *handle)
    : RESTCall(HTTPMethod::POST, COMPUTATION_URL), a(a), b(b), c(c), handle(handle) {}

bool RestCompute::buildRequest(QJsonObject &_json) {
    _json.insert(PARAM_A, a);
    _json.insert(PARAM_B, b);
    _json.insert(PARAM_C, c);
    return true;
}

void RestCompute::handleResponse(const QJsonObject &_json) {
    std::cout << "Got a response" << std::endl;

    QJsonValue v;
    if (checkValue(_json, "result", v)) {
        // This is strange to extract a double, and then to cast it...
        auto result = static_cast<uint32_t>(v.toInt());
        handle->gotResult(result);
    }
}





MathComputer::Status MathComputerProxyClient::compute(uint32_t a, uint32_t b, uint32_t c)
{
    QByteArray block;
    QTextStream out(&block, QIODevice::WriteOnly);

    out << "{compute:" << a << "," << b << "," << c << "}\n";
    out.flush();

    auto restCall = new RestCompute(a, b, c, this);
    sendRequest(restCall);

    return Status::Ok;
}


void MathComputerProxyClient::initConnection()
{
}

MathComputerProxyClient::MathComputerProxyClient()
{
    m_networkManager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    connect(&m_networkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onResult(QNetworkReply *)));
}


void MathComputerProxyClient::connectToHost(QString addr, uint16_t port)
{
    m_ipAddress = addr;
    m_port = port;
}


QString MathComputerProxyClient::makeUrl(const QString &_method) {
    return QString("http://%1:%2/%3").arg(m_ipAddress.toLatin1().data(), QString::number(m_port), _method);
}



void MathComputerProxyClient::newConnectionReady()
{
    initConnection();
}


void MathComputerProxyClient::sendRequest(RESTCall *_pCall) {
    if (_pCall != nullptr) {
        QJsonObject jsonObj;
        if (_pCall->buildRequest(jsonObj)) {
            QNetworkRequest request(makeUrl(_pCall->getUrlPath()));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            request.setOriginatingObject(_pCall);

            if (_pCall->getMethod() == HTTPMethod::POST) {
                QJsonDocument jsonDoc(jsonObj);
                QByteArray data = jsonDoc.toJson(QJsonDocument::Compact);
                qInfo("Sending REST %s : %s", _pCall->getUrlPath().toLatin1().data(), data.data());
                m_networkManager.post(request, data);

                /* Without this, when many requests are sent, some do not arrive (e.g., setting all thresholds).
                 * This fix will check if we do "set" requests (which may come as a group of many requests) and
                 * will wait for the individual requests to finish before sending new ones with an eventloop.
                 * This does not freeze the UI. */
                if (_pCall->getUrlPath().toLatin1().contains("set")) {
                    QEventLoop loop;
                    QObject::connect(&m_networkManager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
                    loop.exec();
                    QObject::disconnect(&m_networkManager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
                }
            } else {
                qInfo("Sending REST %s", _pCall->getUrlPath().toLatin1().data());
                m_networkManager.get(request);
            }
        }
    }
}

void MathComputerProxyClient::onResult(QNetworkReply *reply) {
    RESTCall *pCall = dynamic_cast<RESTCall *>(reply->request().originatingObject());
    if (pCall == nullptr) {
        qDebug() << "No RESTCall instance found for URL " << reply->url().toDisplayString();
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QString data = (QString)reply->readAll();
        QJsonParseError parseError{};
        QJsonDocument doc = QJsonDocument::fromJson(data.toLatin1(), &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            pCall->handleResponse(doc.object());
        } else {
            pCall->handleError(parseError);
        }
    } else {
        pCall->handleError(reply->error(), reply->errorString());
    }

    delete pCall;
}


void MathComputerProxyClient::gotResult(uint32_t result)
{
    emit resultReady(result);
}
