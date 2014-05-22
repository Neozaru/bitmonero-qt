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


//    QNetworkAccessManager network_access_mgr;

//    QUrl endpoint_uri;
//    QNetworkReply* request_reply;
};

#endif // RPCMONERO_H
