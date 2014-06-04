#ifndef RPCMONERO_H
#define RPCMONERO_H


#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>


#include "Models/MoneroModel.h"
#include "Interfaces/MoneroInterface.h"

#include "JsonRPCCommunicator.h"

#include "DaemonHandler.h"


class RPCMonero : public MoneroInterface
{
    Q_OBJECT

public:
    RPCMonero(MoneroModel& pMoneroModel, const WalletSettings& pSettings);
    ~RPCMonero() {}

    bool isReady();

//    WalletModel& getWalletModel();

public slots:
    void getInfo();
//    void daemonRequestFinished();

private:
    JsonRPCCommunicator rpc;
//    WalletModel wallet_model;
    DaemonHandler daemon_handler;

};

#endif // RPCMONERO_H
