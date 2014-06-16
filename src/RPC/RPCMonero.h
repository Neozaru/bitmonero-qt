#ifndef RPCMONERO_H
#define RPCMONERO_H


#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>


#include "Models/MoneroModel.h"
#include "Interfaces/MoneroInterface.h"

#include "JsonRPCCommunicator.h"

#include "DaemonHandler.h"


class RPCMonero : public MoneroInterface
{
    Q_OBJECT
public:
    RPCMonero(MoneroModel& pMoneroModel, const WalletSettings& pSettings);
    ~RPCMonero();

    virtual int enable();

public slots:
    void getInfo();

//    TODO : Move this in another process
    void saveBlockchain();


private:

    DaemonHandler daemon_handler;

    bool should_spawn_daemon;

    JsonRPCCommunicator rpc;

    QTimer getinfo_timer;
    QTimer savebc_timer;

};

#endif // RPCMONERO_H
