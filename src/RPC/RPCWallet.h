#ifndef RPCWALLET_HH
#define RPCWALLET_HH

#include <QJsonArray>
#include <QJsonObject>

#include "Interfaces/WalletInterface.h"


#include "JsonRPCCommunicator.h"
#include "JsonRPCRequest.h"
//#include "Models/WalletModel.h"

class RPCWallet :  public QObject, public WalletInterface
{
    Q_OBJECT
public:
    RPCWallet(WalletModel& pModel, const QString& pHost, unsigned int pPort);

    virtual void getBalance() {
        JsonRPCRequest* req = rpc.sendRequest("getbalance");
        QObject::connect(req,SIGNAL(jsonResponseReceived(QJsonObject)),this,SLOT(balanceResponse(QJsonObject)));
    }

    virtual void transfer(double pAmount, const QString& pAddress) {
        QJsonObject lObj;
        QJsonArray lDests;

        /* Single Dest for the moment */
        QJsonObject lDst;
        lDst["amount"] = pAmount;
        lDst["address"] = pAddress;
        lDests.append(lDst);

        lObj.insert("destinations", lDests);
        rpc.sendRequest("transfer",lObj);
    }

    virtual void store() {
        rpc.sendRequest("store");
    }

    virtual void getPayments(const QString& pPaymentId) {

        QJsonObject lObj;
        lObj.insert("payment_id", pPaymentId);
        rpc.sendRequest("get_payments",lObj);

    }

public slots:
    void balanceResponse(const QJsonObject& obj) {
        qDebug() << "Received !";
        qDebug() << obj;


        if ( obj["unlocked_balance"].isDouble()) {
            onBalanceUpdated(obj["unlocked_balance"].toDouble());
        }
        else {
            qDebug() << "Format error";
        }
    }

private:
    JsonRPCCommunicator rpc;
};

#endif // RPCWALLET_HH
