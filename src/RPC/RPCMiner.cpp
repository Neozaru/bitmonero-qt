#include "RPCMiner.h"

#include <QTimer>

RPCMiner::RPCMiner(MinerModel& pMinerModel, const QString& pHost, unsigned int pPort)
    : MinerInterface(pMinerModel), rpc(pHost,pPort)
{

    getMiningStatus();

    QTimer* lTimer = new QTimer(this);
    QObject::connect(lTimer,SIGNAL(timeout()), this, SLOT(getMiningStatus()));
    lTimer->start(5000);
}

#include <QtCore/QtCore>

void RPCMiner::startMining(const QString& pMoneroAddress, unsigned int pNbThreads) {

    QJsonObject lObj;
    lObj["miner_address"] = pMoneroAddress;
    lObj["threads_count"] = (int) pNbThreads;

    JsonRPCRequest* lReq = rpc.sendRequest("start_mining",lObj, true);
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {
        qDebug() << "'start_mining' Response alt : " << pJsonResponse;
        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {
            this->onStartMiningSucceeded();
        }
        else {
            qWarning() << "Bad status for mining start : " << lStatus;
            this->onStartMiningFailed(lStatus);
        }
    });

}

void RPCMiner::stopMining() {

    JsonRPCRequest* lReq = rpc.sendRequest("stop_mining", QJsonObject(), true);
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {

        qDebug() << "'stop_mining' Response : " << pJsonResponse;
        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {
            this->onStopMiningSucceeded();
        }
        else {
            qWarning() << "Bad status for mining stop : " << lStatus;
            this->onStopMiningFailed(lStatus);
        }

    });

}

void RPCMiner::getMiningStatus() {
    JsonRPCRequest* lReq = rpc.sendRequest("mining_status", QJsonObject(), true);
//    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(getMiningStatusResponse(QJsonObject,QJsonObject)));
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {

        qDebug() << "'mining_status' Response : " << pJsonResponse;
        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {

            if ( !pJsonResponse["active"].isBool() ||
                 !pJsonResponse["threads_count"].isDouble() ||
                 !pJsonResponse["address"].isString() ||
                 !pJsonResponse["speed"].isDouble() ) {

                qCritical() << "Invalid data format for 'mining_status' response. Is your Daemon up to date ?";
            }

            this->onGetMiningStatusResponse(
                    pJsonResponse["active"].toBool(),
                    pJsonResponse["threads_count"].toDouble(),
                    pJsonResponse["address"].toString(),
                    pJsonResponse["speed"].toDouble()
                    );
        }
        else {
            qWarning() << "Bad status for mining status : " << lStatus;
        }

    });
}

