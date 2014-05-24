#ifndef RPCMINER_H
#define RPCMINER_H

#include <QJsonObject>

#include "RPC/JsonRPCCommunicator.h"
#include "RPC/JsonRPCRequest.h"

#include "Interfaces/MinerInterface.h"
#include "Models/MinerModel.h"

class RPCMiner : public QObject, public MinerInterface
{
    Q_OBJECT
public:
    RPCMiner(MinerModel& pMinerModel, const QString& pHost, unsigned int pPort);

    void startMining(const QString& pMoneroAddress, unsigned int pNbThreads) {

        QJsonObject lObj;
        lObj["miner_address"] = pMoneroAddress;
        lObj["threads_count"] = (int) pNbThreads;

        JsonRPCRequest* lReq = rpc.sendRequest("start_mining",lObj, true);
        QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(startMiningResponse(QJsonObject,QJsonObject)));

    }

    void stopMining() {

        JsonRPCRequest* lReq = rpc.sendRequest("stop_mining", QJsonObject(), true);
        QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(stopMiningResponse(QJsonObject,QJsonObject)));

    }

public slots:
    void startMiningResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams) {

        qDebug() << "Response : " << pJsonResponse;
        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {
            qDebug() << "FINE";
            this->onStartMiningSucceeded();
        }
        else {
            qDebug() << "Bad status for mining start : " << lStatus;
            this->onStartMiningFailed(lStatus);
        }

    }

    void stopMiningResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams) {

        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {
            qDebug() << "STOP FINE";
            this->onStopMiningSucceeded();
        }
        else {
            qDebug() << "Bad status for mining stop : " << lStatus;
            this->onStopMiningFailed(lStatus);
        }

    }

private:
    JsonRPCCommunicator rpc;

};

#endif // RPCMINER_H
