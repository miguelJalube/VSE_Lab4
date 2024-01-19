#include "restcall.h"

#include <iostream>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <qlogging.h>



template <class T> inline int sign(T v) { return (v > T(0)) - (v < T(0)); }

RESTCall::RESTCall(HTTPMethod _method, const QString &_url) : m_method(_method), m_url(_url), m_callback(nullptr), m_callbackData(nullptr) {}

HTTPMethod RESTCall::getMethod() { return m_method; }

QString RESTCall::getUrlPath() { return m_url; }

void RESTCall::handleError(const QNetworkReply::NetworkError & /*unused*/, const QString &_errorString) {
    qWarning() << "Error calling REST service '" << m_url << "': HTTP error '" << _errorString << "'";
}

void RESTCall::handleError(const QJsonParseError &_error) {
    qWarning() << "Error calling REST service '" << m_url << "': JSON error '" << _error.errorString() << "'";
}

void RESTCall::handleError(const QString &_error) { qWarning() << "Error calling REST service '" << m_url << "': Response error '" << _error << "'"; }

bool RESTCall::checkValue(const QJsonObject &_parent, const QString &_name, QJsonValue &_value) {
    if (!_parent.contains(_name)) {
        handleError(QString("Missing field '%1'").arg(_name));
        return false;
    }

    _value = _parent.value(_name);
    if (_value.isArray() || _value.isObject() || _value.isUndefined() || _value.isNull()) {
        handleError(QString("Field '%1' has wrong data type").arg(_name));
        return false;
    }

    return true;
}

bool RESTCall::checkValue(const QJsonArray &_parent, int _index, QJsonValue &_value) {
    if (_parent.count() <= _index) {
        handleError(QString("Missing index %1").arg(_index));
        return false;
    }

    _value = _parent.at(_index);
    if (_value.isArray() || _value.isObject() || _value.isUndefined() || _value.isNull()) {
        handleError(QString("Index %1 has wrong data type").arg(_index));
        return false;
    }

    return true;
}

bool RESTCall::checkInt(const QJsonObject &_parent, const QString &_name, int _expectedValue) {
    QJsonValue v;
    if (!checkValue(_parent, _name, v)) {
        return false;
    }

    if (v.isBool() || /* v.isDouble() || */ v.isString()) {
        handleError(QString("Field %1 has wrong data type").arg(_name));
        return false;
    }

    if (v.toInt() != _expectedValue) {
        handleError(QString("Field '%1' has wrong value").arg(_name));
        return false;
    }

    return true;
}

bool RESTCall::checkInt(const QJsonArray &_parent, int _index, int _expectedValue) {
    QJsonValue v;
    if (!checkValue(_parent, _index, v)) {
        return false;
    }

    if (v.isBool() || /* v.isDouble() || */ v.isString()) {
        handleError(QString("Index %1 has wrong data type").arg(_index));
        return false;
    }

    if (v.toInt() != _expectedValue) {
        handleError(QString("Index %1 has wrong value").arg(_index));
        return false;
    }

    return true;
}

bool RESTCall::checkDouble(const QJsonObject &_parent, const QString &_name, double _expectedValue) {
    QJsonValue v;
    if (!checkValue(_parent, _name, v)) {
        return false;
    }

    if (v.isBool() || v.isString()) {
        handleError(QString("Field %1 has wrong data type").arg(_name));
        return false;
    }

    if (v.toDouble() != _expectedValue) {
        handleError(QString("Field '%1' has wrong value").arg(_name));
        return false;
    }

    return true;
}

bool RESTCall::checkDouble(const QJsonArray &_parent, int _index, double _expectedValue) {
    QJsonValue v;
    if (!checkValue(_parent, _index, v)) {
        return false;
    }

    if (v.isBool() || v.isString()) {
        handleError(QString("Index %1 has wrong data type").arg(_index));
        return false;
    }

    if (v.toDouble() != _expectedValue) {
        handleError(QString("Index %1 has wrong value").arg(_index));
        return false;
    }

    return true;
}

bool RESTCall::checkString(const QJsonObject &_parent, const QString &_name, const QString &_expectedValue) {
    QJsonValue v;
    if (!checkValue(_parent, _name, v)) {
        return false;
    }

    if (!v.isString()) {
        handleError(QString("Field '%1' is not a string").arg(_name));
        return false;
    }

    if (v.toString() != _expectedValue) {
        handleError(QString("Value '%1' has wrong value").arg(_name));
        return false;
    }

    return true;
}

bool RESTCall::checkString(const QJsonArray &_parent, int _index, const QString &_expectedValue) {
    QJsonValue v;
    if (!checkValue(_parent, _index, v)) {
        return false;
    }

    if (!v.isString()) {
        handleError(QString("Index '%1' is not a string").arg(_index));
        return false;
    }

    if (v.toString() != _expectedValue) {
        handleError(QString("Index %1 has wrong value").arg(_index));
        return false;
    }

    return true;
}

bool RESTCall::checkObject(const QJsonObject &_parent, const QString &_name, QJsonObject &_obj) {
    if (!_parent.contains(_name)) {
        handleError(QString("Missing object '%1'").arg(_name));
        return false;
    }

    if (!_parent.value(_name).isObject()) {
        handleError(QString("Field '%1' is not an object").arg(_name));
        return false;
    }
    _obj = _parent.value(_name).toObject();
    return true;
}

bool RESTCall::checkObject(const QJsonArray &_parent, int _index, QJsonObject &_obj) {
    if (_parent.count() <= _index) {
        handleError(QString("Missing index %1").arg(_index));
        return false;
    }

    if (!_parent.at(_index).isObject()) {
        handleError(QString("Index %1 is not an object").arg(_index));
        return false;
    }
    _obj = _parent.at(_index).toObject();
    return true;
}

bool RESTCall::checkArray(const QJsonObject &_parent, const QString &_name, int _expectedSize, QJsonArray &_array) {
    if (!_parent.contains(_name)) {
        handleError(QString("Missing array '%1'").arg(_name));
        return false;
    }

    if (!_parent.value(_name).isArray()) {
        handleError(QString("Field '%1' is not an array").arg(_name));
        return false;
    }

    _array = _parent.value(_name).toArray();
    if (_array.size() != _expectedSize && _expectedSize > -1) {
        handleError(QString("Wrong array size '%1'").arg(_name));
        return false;
    }

    return true;
}

bool RESTCall::checkArray(const QJsonArray &_parent, int _index, int _expectedSize, QJsonArray &_array) {
    if (_parent.count() <= _index) {
        handleError(QString("Missing index %1").arg(_index));
        return false;
    }

    if (!_parent.at(_index).isArray()) {
        handleError(QString("Index %1 is not an array").arg(_index));
        return false;
    }

    _array = _parent.at(_index).toArray();
    if (_array.size() != _expectedSize) {
        handleError(QString("Wrong array size for child at index %1").arg(_index));
        return false;
    }

    return true;
}


void RESTCall::setCallback(TRESTCallback _callback, void *_data) {
    m_callback = _callback;
    m_callbackData = _data;
}

void RESTCall::runCallback() {
    if (m_callback != nullptr) {
        m_callback(this, m_callbackData);
    }
}
