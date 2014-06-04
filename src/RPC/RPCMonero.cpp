#include "RPCMonero.h"

#include <QTimer>

#include <QString>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QEventLoop>

#include <QJsonObject>

#include "JsonRPCRequest.h"

RPCMonero::RPCMonero(MoneroModel& pMoneroModel, const WalletSettings& pSettings)
    : MoneroInterface(pMoneroModel), rpc(pSettings.getMoneroUri(),pSettings.getMoneroPort()), daemon_handler(pSettings)
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
        else {
            qDebug() << "DAEMON STARTED";
        }

    }


    QTimer* lGetInfoTimer = new QTimer();
    QObject::connect(lGetInfoTimer, SIGNAL(timeout()), this, SLOT(getInfo()));
    lGetInfoTimer->start(5000);


    /* TODO : Move in another process (daemon itself ?) */
    QTimer* lSaveBlockchainTimer = new QTimer();
    QObject::connect(lSaveBlockchainTimer, SIGNAL(timeout()), this, SLOT(saveBlockchain()));
    lSaveBlockchainTimer->start(90000);
}



void RPCMonero::getInfo()
{

    JsonRPCRequest* lReq = rpc.sendRequest("getinfo",QJsonObject(), true);
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {

        qDebug() << "'getinfo' Response alt : " << pJsonResponse;
        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {

            this->onInfoUpdated(
                pJsonResponse["height"].toInt(),
                pJsonResponse["target_height"].toInt(),
                pJsonResponse["difficulty"].toInt(),
                pJsonResponse["incoming_connections_count"].toInt(),
                pJsonResponse["outgoing_connections_count"].toInt()
            );

        }
        else {
            qWarning() << "Bad status for 'getinfo' start : " << lStatus;

        }
    });

}

void RPCMonero::saveBlockchain()
{

    JsonRPCRequest* lReq = rpc.sendRequest("save_bc",QJsonObject(), true);
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {
        //NOP
    });

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

