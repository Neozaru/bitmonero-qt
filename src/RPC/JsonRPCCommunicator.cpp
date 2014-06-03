#include "JsonRPCCommunicator.h"

JsonRPCCommunicator::JsonRPCCommunicator(const QString& pHost, unsigned int pPort, const QString& pUri, const QString& pRpcVersion)
    : rpc_version(pRpcVersion)
{
    endpoint_uri = QUrl("http://" + pHost + pUri);
    endpoint_uri.setPort(pPort);
}

JsonRPCCommunicator::~JsonRPCCommunicator() {

}

JsonRPCRequest* JsonRPCCommunicator::sendRequest(const QString& pMethod, const QJsonObject& pParams, bool pDaemonHttp)
{

    QUrl lUrl = endpoint_uri;
    QJsonObject lJsonRoot;

    /* Depending of RPC type, will append method name in URI or in JSON data */
    if ( pDaemonHttp ) {
        lJsonRoot = pParams;
        lUrl.setPath("/"+pMethod);
    }
    else {
        lJsonRoot.insert("jsonrpc",QJsonValue::fromVariant(rpc_version));
        lJsonRoot.insert("method",QJsonValue::fromVariant(pMethod));
        if ( !pParams.empty() ) {
            lJsonRoot.insert("params", pParams);
        }
    }

    QJsonDocument lJsonDoc;
    lJsonDoc.setObject(lJsonRoot);

    QNetworkRequest lReq(lUrl);
    lReq.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    qDebug() << "Req" << lReq.url();
    qDebug() << lJsonDoc.toJson();


    JsonRPCRequest* lJsonReq = new JsonRPCRequest(pParams, pDaemonHttp);
    QNetworkReply* lReply = network_access_mgr.post(lReq, lJsonDoc.toJson());

    QObject::connect(lReply, SIGNAL(finished()), lJsonReq, SLOT(onRequestFinished()));
    QObject::connect(lReply, SIGNAL(error(QNetworkReply::NetworkError)), lJsonReq, SLOT(onRequestError(QNetworkReply::NetworkError)));


    return lJsonReq;

}

