#include "JsonRPCRequest.h"

#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>


JsonRPCRequest::JsonRPCRequest(const QJsonObject& pOriginalParams, bool pDaemonHttp) : original_params(pOriginalParams), daemon_http(pDaemonHttp)
{
}



void JsonRPCRequest::onRequestFinished() {

    QNetworkReply* lReply = qobject_cast<QNetworkReply*>(sender());
    if ( lReply->error() != QNetworkReply::NoError ) {
        qDebug() << "Error : " << lReply->error();
        qDebug() << lReply->readAll();
    }
    else {
        QByteArray lData =     lReply->readAll();
        qDebug() << "Server response : ";
        qDebug() << QString(lData);
        QJsonDocument lResJson = QJsonDocument::fromJson(lData);
        QJsonObject lJsonObj;

        if ( daemon_http ) {
            lJsonObj = lResJson.object();
        }
        else {
            lJsonObj = lResJson.object()["result"].toObject();
        }

        error = QNetworkReply::NetworkError::NoError;
        emit jsonResponseReceived(lJsonObj,original_params);

    }

}


void JsonRPCRequest::onRequestError(const QNetworkReply::NetworkError& pError) {

    qDebug() << "Error occured with Json Request";
    error = pError;

    emit errorOccured(pError);
}

