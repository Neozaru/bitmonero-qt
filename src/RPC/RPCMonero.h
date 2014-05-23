#ifndef RPCMONERO_H
#define RPCMONERO_H


#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>

#include "Interfaces/MoneroInterface.h"

#include "JsonRPCCommunicator.h"

class RPCMonero : public MoneroInterface
{
    Q_OBJECT

public:
    RPCMonero(const QString& pHost = "localhost", unsigned int pPort = 18081 );
    virtual ~RPCMonero() {}

    virtual WalletModel& getWalletModel();

public slots:
    void update();
    void daemonRequestFinished();

private:
    JsonRPCCommunicator rpc;
    WalletModel wallet_model;

};

#endif // RPCMONERO_H
