#ifndef RPCMINER_H
#define RPCMINER_H

#include <QJsonObject>
#include <QTimer>

#include "RPC/JsonRPCCommunicator.h"
#include "RPC/JsonRPCRequest.h"

#include "Interfaces/MinerInterface.h"
#include "Models/MinerModel.h"

#include "WalletSettings.h"

class RPCMiner : public QObject, public MinerInterface
{
    Q_OBJECT
public:
    RPCMiner(MinerModel& pMinerModel, const WalletSettings& pSettings);
    ~RPCMiner();

    int enable();

    void startMining(const QString& pMoneroAddress, unsigned int pNbThreads);
    void stopMining();


public slots:
    void getMiningStatus();

private:
    JsonRPCCommunicator rpc;

    QTimer miningstatus_timer;

};

#endif // RPCMINER_H
