#ifndef JSONRPCREQUEST_H
#define JSONRPCREQUEST_H

#include <QObject>
#include <QJsonObject>

class JsonRPCRequest : public QObject
{
    Q_OBJECT
public:
    JsonRPCRequest();

public slots:
    void onRequestFinished();

signals:
    void jsonResponseReceived(QJsonObject);
};

#endif // JSONRPCREQUEST_H
