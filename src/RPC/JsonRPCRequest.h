#ifndef JSONRPCREQUEST_H
#define JSONRPCREQUEST_H

#include <QObject>
#include <QJsonObject>

class JsonRPCRequest : public QObject
{
    Q_OBJECT
public:
    JsonRPCRequest(const QJsonObject& pOriginalParams = QJsonObject());

public slots:
    void onRequestFinished();

signals:
    void jsonResponseReceived(QJsonObject,QJsonObject);

private:
    QJsonObject original_params;
};

#endif // JSONRPCREQUEST_H
