#ifndef PROCESSINGCLIENT_H
#define PROCESSINGCLIENT_H

#include <cstdint>

#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QObject>

#include "restcall.h"

#define COMPUTATION_URL "computation"
#define NBCOMPUTE_URL "nbcompute"
#define RESETNBCOMPUTE_URL "resetnbcompute"


// Computation parameters
#define PARAM_A      "a"
#define PARAM_B      "b"
#define PARAM_C      "c"


class MathComputerProxyClient;

// Configures the stimulation settings in the PS. If fire is set to true, will start immediately the stimulation.
class RestCompute : public RESTCall {
public:
    RestCompute(uint16_t a, uint16_t b, uint16_t c, MathComputerProxyClient *handle);

public:
    bool buildRequest(QJsonObject &_json) override;
    void handleResponse(const QJsonObject &_json) override;

private:
    uint16_t a;
    uint16_t b;
    uint16_t c;

    MathComputerProxyClient *handle;
};

// Configures the stimulation settings in the PS. If fire is set to true, will start immediately the stimulation.
class RestGetNbCompute : public RESTCall {
public:
    RestGetNbCompute(MathComputerProxyClient *handle);

public:
    bool buildRequest(QJsonObject &_json) override;
    void handleResponse(const QJsonObject &_json) override;

private:

    MathComputerProxyClient *handle;
};


class RestResetNbCompute : public RESTCall {
public:
    RestResetNbCompute(MathComputerProxyClient *handle);

public:
    bool buildRequest(QJsonObject &_json) override;
    void handleResponse(const QJsonObject &_json) override;

private:

    MathComputerProxyClient *handle;
};


class MathComputer : public QObject
{
    Q_OBJECT

public:
    MathComputer() = default;
    virtual ~MathComputer() = default;

    enum class Status {
        Ok,
        Error
    };

public slots:

    virtual Status compute(uint32_t a, uint32_t b, uint32_t c) = 0;

signals:
    void resultReady(uint32_t result);
    void nbComputeReady(uint32_t result);
};


class MathComputerProxyClient : public MathComputer
{
    Q_OBJECT

public:
    MathComputerProxyClient();
    ~MathComputerProxyClient() override = default;

    void initConnection();

    void connectToHost(QString addr, uint16_t port);


public slots:
    void onResult(QNetworkReply *reply);
    Status compute(uint32_t a, uint32_t b, uint32_t c) override;
    void gotResult(uint32_t result);

    void getNbCompute();
    void resetNbCompute();
    void gotNbCompute(uint32_t result);

private slots:
    void newConnectionReady();


private:
    void sendRequest(RESTCall *_pCall);
    QString makeUrl(const QString &_method);


    QString m_ipAddress;
    int m_port;
    QTcpSocket *connection{nullptr};
    QNetworkAccessManager m_networkManager;

};


#endif // PROCESSINGCLIENT_H
