#include "RPCMiner.h"

RPCMiner::RPCMiner(MinerModel& pMinerModel, const QString& pHost, unsigned int pPort)
    : MinerInterface(pMinerModel), rpc(pHost,pPort)
{
}


void RPCMiner::startMining(const QString& pMoneroAddress, unsigned int pNbThreads) {

    QJsonObject lObj;
    lObj["miner_address"] = pMoneroAddress;
    lObj["threads_count"] = (int) pNbThreads;

    JsonRPCRequest* lReq = rpc.sendRequest("start_mining",lObj, true);
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(startMiningResponse(QJsonObject,QJsonObject)));

}

void RPCMiner::stopMining() {

    JsonRPCRequest* lReq = rpc.sendRequest("stop_mining", QJsonObject(), true);
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(stopMiningResponse(QJsonObject,QJsonObject)));

}

void RPCMiner::startMiningResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams) {

    qDebug() << "'start_mining' Response : " << pJsonResponse;
    const QString& lStatus = pJsonResponse["status"].toString();

    if ( lStatus == "OK" ) {
        this->onStartMiningSucceeded();
    }
    else {
        qWarning() << "Bad status for mining start : " << lStatus;
        this->onStartMiningFailed(lStatus);
    }

}

void RPCMiner::stopMiningResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams) {

    qDebug() << "'stop_mining' Response : " << pJsonResponse;
    const QString& lStatus = pJsonResponse["status"].toString();

    if ( lStatus == "OK" ) {
        this->onStopMiningSucceeded();
    }
    else {
        qWarning() << "Bad status for mining stop : " << lStatus;
        this->onStopMiningFailed(lStatus);
    }

}
