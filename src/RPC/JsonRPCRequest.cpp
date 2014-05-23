#include "JsonRPCRequest.h"

#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>


JsonRPCRequest::JsonRPCRequest(const QJsonObject& pOriginalParams) : original_params(pOriginalParams)
{
}



void JsonRPCRequest::onRequestFinished() {

    QNetworkReply* lReply = qobject_cast<QNetworkReply*>(sender());
    if ( lReply->error() != QNetworkReply::NoError ) {
        qDebug() << "Error : " << lReply->error();
    }
    else {
        QByteArray lData =     lReply->readAll();
        qDebug() << "Server response : ";
        qDebug() << QString(lData);
        QJsonDocument lResJson = QJsonDocument::fromJson(lData);
        QJsonObject lJsonObj = lResJson.object()["result"].toObject();

        emit jsonResponseReceived(lJsonObj,original_params);

    }

}
