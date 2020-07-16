#pragma once
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>
#include <QEventLoop>
#include <QFile>
#include <QBuffer>
#include "qcustomnetworkreply.h"

class AccessManager: public QNetworkAccessManager
{
    Q_OBJECT
public:
    AccessManager(QObject* obj  = nullptr) : QNetworkAccessManager(obj)
    {
    }

    virtual QNetworkReply *
    createRequest(QNetworkAccessManager::Operation op,
                  const QNetworkRequest &originalReq,
                  QIODevice *outgoingData = nullptr) override
    {

        if(outgoingData)
        {
            auto length = originalReq.header(QNetworkRequest::ContentLengthHeader).toInt();
            auto data = outgoingData->peek(length);
            QStringList params = QString(data).split('&');
            paramsLoaded(params);
        }
        auto path = originalReq.url().path();
        if(path == "/binary/")
        {
            //change first reply for prevent loading mobile version
            QCustomNetworkReply *reply = new QCustomNetworkReply();
            reply->setHttpStatusCode( 200, "Ok" );
            reply->setContentType("text/html");
            QFile replyBodyFile(":/resources/first_response.txt");
            replyBodyFile.open(QIODevice::ReadOnly);
            reply->setContent(replyBodyFile.readAll());
            replyBodyFile.close();
            return reply;
        }
        if(path == "/binary/a-1.18.14.js")
        {
            //change js for access to websocket
            QCustomNetworkReply *reply = new QCustomNetworkReply();
            reply->setHttpStatusCode( 200, "Ok" );
            reply->setContentType("*/*");
            QFile replyBodyFile(":/resources/a-1.18.14.js");
            replyBodyFile.open(QIODevice::ReadOnly);
            reply->setContent(replyBodyFile.readAll());
            replyBodyFile.close();
            return reply;
        }
        return QNetworkAccessManager::createRequest(op,originalReq,outgoingData);
    }
    ~AccessManager(){}
signals:
    void paramsLoaded(const QStringList &params);
};
