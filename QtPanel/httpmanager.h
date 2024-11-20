/******************************************************************************
 *
 * @file       httpmanager.h
 * @brief      send and receive http request
 *
 * @author     KBchulan
 * @date       2024/11/20
 * @history
 *****************************************************************************/
#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include "singleton.h"

#include <QUrl>
#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>

class HttpManager final : public Singleton<HttpManager>, public QObject, public std::enable_shared_from_this<HttpManager>
{
    Q_OBJECT
    friend class Singleton<HttpManager>;

public:
    ~HttpManager();

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

signals:
    // when the server response, send this signals
    void sig_http_finished(ReqId id, QString res, ErrorCodes err, Modules mod);

private:
    HttpManager();

    // send http request
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);

private:
    QNetworkAccessManager _manager;
};

#endif // HTTPMANAGER_H
