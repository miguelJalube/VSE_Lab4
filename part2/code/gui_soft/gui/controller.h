#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class QQmlContext;
class QQmlApplicationEngine;
class MathComputerProxyClient;

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    QObject *getRootObject();

public slots:
    void setA(QString aText);
    void setB(QString bText);
    void setC(QString cText);
    void compute();
    void getNbCompute();
    void resetNbCompute();
    void connect();

    void resultReady(uint32_t result);
    void NbComputeReady(uint32_t nbCompute);

signals:

    void getResult(QString result);
    void changeStatus(QString status);
    void getNbCompute(QString nbCompute);

private:
    QString m_a;
    QString m_b;
    QString m_c;

    MathComputerProxyClient *client{nullptr};

public:
#ifdef CONFIG_GUITEST
    QQmlContext *m_rootContext;
    QQmlApplicationEngine *engine;
#endif // CONFIG_GUITEST
};

#endif // CONTROLLER_H
