#ifndef JSONRPCREQUEST_H
#define JSONRPCREQUEST_H

#include <QObject>
#include <QJsonObject>

class JsonRPCRequest : public QObject
{
    Q_OBJECT
public:
    JsonRPCRequest(const QJsonObject& pOriginalParams = QJsonObject(), bool pDaemonHttp = false);

public slots:
    void onRequestFinished();

signals:
    void jsonResponseReceived(QJsonObject,QJsonObject);

private:
    QJsonObject original_params;
    bool daemon_http;
};

#endif // JSONRPCREQUEST_H
