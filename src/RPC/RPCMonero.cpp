#include "RPCMonero.h"

#include <QTimer>

#include <QString>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


RPCMonero::RPCMonero(const QString& pHost, unsigned int pPort) : rpc(pHost,pPort)
{
    //TODO
    QTimer *timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5000);

}


WalletModel& RPCMonero::getWalletModel()
{
    return wallet_model;
}


void RPCMonero::update()
{


}


void RPCMonero::daemonRequestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if ( !reply->isReadable() ) {
        qDebug() << "Not readable";
    }
    else if ( reply->error() != QNetworkReply::NoError ) {
        qDebug() << "Error : " << reply->error();
    }
    else {
        QByteArray data =     reply->readAll();
        qDebug() << QString(data);
        QJsonDocument res_json = QJsonDocument::fromJson(data);
        qDebug() << res_json.object()["result"];
        QJsonObject result = res_json.object()["result"].toObject();

        int count = result["count"].toInt();


        wallet_model.setBalance( count );

    }



    //TODO : Delete reply
}
