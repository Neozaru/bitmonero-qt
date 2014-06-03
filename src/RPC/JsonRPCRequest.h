#ifndef JSONRPCREQUEST_H
#define JSONRPCREQUEST_H

#include <QObject>
#include <QJsonObject>

#include <QNetworkReply>

class JsonRPCRequest : public QObject
{
    Q_OBJECT
public:
    JsonRPCRequest(const QJsonObject& pOriginalParams = QJsonObject(), bool pDaemonHttp = false);

    const QNetworkReply::NetworkError& getError() const {
        return error;
    }


public slots:
    void onRequestFinished();
    void onRequestError(const QNetworkReply::NetworkError& pError);

signals:
    void jsonResponseReceived(QJsonObject,QJsonObject);
    void errorOccured(QNetworkReply::NetworkError);

private:
    QJsonObject original_params;
    bool daemon_http;

    QNetworkReply::NetworkError error;
};

#endif // JSONRPCREQUEST_H
