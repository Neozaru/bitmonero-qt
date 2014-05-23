#include "JsonRPCCommunicator.h"

JsonRPCCommunicator::JsonRPCCommunicator(const QString& pHost, unsigned int pPort, const QString& pUri, const QString& pRpcVersion)
    : rpc_version(pRpcVersion)
{
    endpoint_uri = QUrl("http://" + pHost + pUri);
    endpoint_uri.setPort(pPort);
}

JsonRPCCommunicator::~JsonRPCCommunicator() {

}

JsonRPCRequest* JsonRPCCommunicator::sendRequest(const QString& pMethod, const QJsonObject& pParams )
{

    QJsonObject lJsonRoot;
    lJsonRoot.insert("jsonrpc",QJsonValue::fromVariant(rpc_version));
    lJsonRoot.insert("method",QJsonValue::fromVariant(pMethod));
    if ( !pParams.empty() ) {
        lJsonRoot.insert("params", pParams);
    }

    QJsonDocument lJsonDoc;
    lJsonDoc.setObject(lJsonRoot);
    qDebug() << lJsonDoc.toJson();

    QNetworkRequest lReq(endpoint_uri);
    lReq.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    qDebug() << lReq.url();


    JsonRPCRequest* lJsonReq = new JsonRPCRequest(pParams);
    QNetworkReply* lReply = network_access_mgr.post(lReq,lJsonDoc.toJson());
//    lReply->setUserData(0,(QObjectUserData*)lJsonReq);

    QObject::connect(lReply, SIGNAL(finished()), lJsonReq, SLOT(onRequestFinished()));


    return lJsonReq;

}


void JsonRPCCommunicator::onRequestFinished() {

}
