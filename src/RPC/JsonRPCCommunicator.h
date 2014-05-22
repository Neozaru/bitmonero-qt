#ifndef JSONRPCCOMMUNICATOR_HH
#define JSONRPCCOMMUNICATOR_HH

#include <string>

#include <QObject>
#include <QString>
#include <QUrl>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QJsonObject>
#include <QJsonDocument>

//class JsonRPCCommunicator;
#include "JsonRPCRequest.h"

class JsonRPCCommunicator : public QObject
{
    Q_OBJECT
public:
    JsonRPCCommunicator(const QString& pHost = "localhost", unsigned int pPort = 80, const QString& pUri = "/json_rpc", const QString& pRpcVersion = "2.0");
    ~JsonRPCCommunicator();

    JsonRPCRequest* sendRequest(const QString& pMethod, const QJsonObject& pParams = QJsonObject());

public slots:
    void onRequestFinished();

signals:
    void jsonResponseReceived();

private:

    QString rpc_version;
    QUrl endpoint_uri;

    QNetworkAccessManager network_access_mgr;

};

#endif // JSONPRCCOMMUNICATOR_HH
