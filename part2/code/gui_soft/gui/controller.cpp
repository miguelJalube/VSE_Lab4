#include <iostream>

#include <QQmlApplicationEngine>

#include "controller.h"

#include "processingclient.h"

Controller::Controller(QObject *parent) : QObject(parent)
{

}


void Controller::setA(QString aText)
{
    std::cout << "Sets A : " << qPrintable(aText) << std::endl;
    m_a = aText;
}

void Controller::setB(QString bText)
{
    std::cout << "Sets B : " << qPrintable(bText) << std::endl;
    m_b = bText;
}
void Controller::setC(QString cText)
{
    std::cout << "Sets C : " << qPrintable(cText) << std::endl;
    m_c = cText;
}
void Controller::compute()
{
    std::cout << "Start computation" << std::endl;
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    bool ok = true;
    a = m_a.toUInt(&ok);
    if (!ok) {
        emit changeStatus("Error with A: Not an unsigned int");
        return;
    }
    b = m_b.toUInt(&ok);
    if (!ok) {
        emit changeStatus("Error with B: Not an unsigned int");
        return;
    }
    c = m_c.toUInt(&ok);
    if (!ok) {
        emit changeStatus("Error with C: Not an unsigned int");
        return;
    }

    emit changeStatus("Start computation");

    connect();
    client->compute(a, b, c);
}

void Controller::resultReady(uint32_t result)
{
    QString text = QString("%1").arg(result);
    emit getResult(text);
    emit changeStatus("Got result");
}

void Controller::getNbCompute()
{
    connect();
    client->getNbCompute();
}

void Controller::resetNbCompute()
{
    connect();
    client->resetNbCompute();
}

void Controller::nbComputeReady(uint32_t nbCompute)
{
    std::cout << "Controller::nbComputeReady: " << nbCompute << std::endl;
    emit gotNbCompute(QString("%1").arg(nbCompute));
}

void Controller::connect()
{
    std::cout << "Connect" << std::endl;
    if (client != nullptr) {
        return;
    }
    client = new MathComputerProxyClient();
    if (!QObject::connect(client, &MathComputerProxyClient::resultReady, this, &Controller::resultReady)) {
        std::cout << "Error with connection" << std::endl;
    }
    if (!QObject::connect(client, &MathComputerProxyClient::nbComputeReady, this, &Controller::nbComputeReady)) {
        std::cout << "Error with connection" << std::endl;
    }
    client->connectToHost("localhost", 12345);
}


// Only used for automated tests
QObject* Controller::getRootObject()
{
#ifdef CONFIG_GUITEST
    return engine->rootObjects().at(0);
#else
    assert(false);
    return nullptr;
    //return rootObject();
#endif // CONFIG_GUITEST
}
