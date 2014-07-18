#include "JsonRPCRequest.h"

#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>


JsonRPCRequest::JsonRPCRequest(const QJsonObject& pOriginalParams, bool pDaemonHttp) : original_params(pOriginalParams), daemon_http(pDaemonHttp)
{
}



void JsonRPCRequest::onRequestFinished() {

    QNetworkReply* lReply = qobject_cast<QNetworkReply*>(sender());
    if ( lReply->error() != QNetworkReply::NoError ) {
//        qDebug() << "Error : " << lReply->error();
//        qDebug() << lReply->readAll();
    }
    else {
        QByteArray lData = lReply->readAll();
        delete lReply;
//        qDebug() << "Server response : ";
//        qDebug() << QString(lData).left(1024);
        QJsonDocument lResJson = QJsonDocument::fromJson(lData);
        QJsonObject lJsonObj;

        if ( daemon_http ) {
            lJsonObj = lResJson.object();
        }
        else {
            /* TODO : Handle errors here and not in RPCWallet/Miner */
            if ( lResJson.object()["result"].isObject() ) {
                lJsonObj = lResJson.object()["result"].toObject();
            }
            /* Hack (sends the whole object) */
            else if ( lResJson.object()["error"].isObject() ) {
//                qWarning() << "Received RPC error status, forwarding whole object";
                lJsonObj = lResJson.object();
            }
        }

        error = QNetworkReply::NetworkError::NoError;
        emit jsonResponseReceived(lJsonObj,original_params);

        /* Don't use a request after processing events */
        delete this;

    }

}


void JsonRPCRequest::onRequestError(const QNetworkReply::NetworkError& pError) {

//    qDebug() << "Error occured with Json Request";
    error = pError;

    emit errorOccured(pError);
}


