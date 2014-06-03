#include "RPCMonero.h"

#include <QTimer>

#include <QString>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QEventLoop>


RPCMonero::RPCMonero(const WalletSettings& pSettings)
    : rpc(pSettings.getMoneroUri(),pSettings.getMoneroPort()), daemon_handler(pSettings)
{

    if (pSettings.shouldSpawnDaemon()) {

//        daemon_handler = DaemonHandler(pSettings);

        if (!daemon_handler.isOk()) {
            qCritical() << "Configured to spawn 'bitmonerod' daemon but no executable found. Aborting...";
//            throw std::l;
//            TODO: throw
            return;
        }

        if (!daemon_handler.execDaemon()) {
            qCritical() << "'bitmonerod' Daemon start failed. Is '" << daemon_handler.getDaemonProgram() << "' the daemon executable ?";
//            TODO: throw
            return;
        }
        /* Not sure */
        else {
            qDebug() << "DAEMON STARTED";
        }

    }

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

bool RPCMonero::isReady() {

    JsonRPCRequest* lReq = rpc.sendRequest("getheight",QJsonObject(), true);

    /* Synchronous call */
    QEventLoop loop;
    QObject::connect(lReq, SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)), &loop, SLOT(quit()));
    QObject::connect(lReq, SIGNAL(errorOccured(QNetworkReply::NetworkError)), &loop, SLOT(quit()));

    loop.exec();

    return lReq->getError() == QNetworkReply::NetworkError::NoError;

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
