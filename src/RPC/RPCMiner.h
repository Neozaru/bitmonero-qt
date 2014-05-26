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

    void startMining(const QString& pMoneroAddress, unsigned int pNbThreads);
    void stopMining();

public slots:
    void getMiningStatus();

    void startMiningResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams);
    void stopMiningResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams);
    void getMiningStatusResponse(const QJsonObject& pJsonResponse, const QJsonObject& pOriginalParams);

private:
    JsonRPCCommunicator rpc;

};

#endif // RPCMINER_H
