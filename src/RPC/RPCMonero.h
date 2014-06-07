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

    bool isReady();

    virtual void enable();

public slots:
    void getInfo();

//    TODO : Move this in another process
    void saveBlockchain();


private:
    JsonRPCCommunicator rpc;

    DaemonHandler daemon_handler;

    bool should_spawn_daemon;

};

#endif // RPCMONERO_H
