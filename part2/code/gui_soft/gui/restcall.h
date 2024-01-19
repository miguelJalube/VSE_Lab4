#ifndef RESTCALL_H
#define RESTCALL_H

#include <QObject>
#include <QNetworkReply>

class QJsonValue;
class QJsonArray;
class QJsonObject;
struct QJsonParseError;

class RESTCall;



// typedef void (*TRESTCallback)(const RESTCall *, void*);
using TRESTCallback = void (*)(const RESTCall *, void *);

enum class HTTPMethod { GET, POST };

class RESTCall : public QObject {
    Q_OBJECT
public:
    RESTCall(HTTPMethod _method, const QString &_url);

public:
    HTTPMethod getMethod();
    QString getUrlPath();
    virtual bool buildRequest(QJsonObject &_json) = 0;
    virtual void handleResponse(const QJsonObject &_json) = 0;
    virtual void handleError(const QNetworkReply::NetworkError &_error, const QString &_errorString);
    virtual void handleError(const QJsonParseError &_error);
    virtual void handleError(const QString &_error);

protected:
    bool checkValue(const QJsonObject &_parent, const QString &_name, QJsonValue &_value);
    bool checkValue(const QJsonArray &_parent, int _index, QJsonValue &_value);
    bool checkInt(const QJsonObject &_parent, const QString &_name, int _expectedValue);
    bool checkInt(const QJsonArray &_parent, int _index, int _expectedValue);
    bool checkDouble(const QJsonObject &_parent, const QString &_name, double _expectedValue);
    bool checkDouble(const QJsonArray &_parent, int _index, double _expectedValue);
    bool checkString(const QJsonObject &_parent, const QString &_name, const QString &_expectedValue);
    bool checkString(const QJsonArray &_parent, int _index, const QString &_expectedValue);
    bool checkObject(const QJsonObject &_parent, const QString &_name, QJsonObject &_obj);
    bool checkObject(const QJsonArray &_parent, int _index, QJsonObject &_obj);
    bool checkArray(const QJsonObject &_parent, const QString &_name, int _expectedSize, QJsonArray &_array);
    bool checkArray(const QJsonArray &_parent, int _index, int _expectedSize, QJsonArray &_array);

    void setCallback(TRESTCallback _callback, void *_data);
    void runCallback();

private:
    HTTPMethod m_method;
    QString m_url;
    TRESTCallback m_callback;
    void *m_callbackData;
};

#endif // RESTCALL_H
