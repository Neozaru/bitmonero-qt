#include "JsonRPCRequest.h"

#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>


JsonRPCRequest::JsonRPCRequest()
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

//        if ( lJsonObj["status"].isString() &&
//             lJsonObj["status"].toString().compare("OK") == 0 ) {

            emit jsonResponseReceived(lJsonObj);
//        }
//        else {
//            qDebug() << "That's an error !";
//        }

    }

}
