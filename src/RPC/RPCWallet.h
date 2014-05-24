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

    void transfer(double pAmount, const QString& pAddress, int pFee) {
        QJsonObject lObj;
        QJsonArray lDests;

        /* Single Dest for the moment */
        QJsonObject lDst;
        lDst["amount"] = pAmount;
        lDst["address"] = pAddress;
        lDests.append(lDst);
        lObj["destinations"] = lDests;

        lObj["fee"] = pFee;

        /* TODO */
        lObj["mixin"]= 0;
        lObj["unlock_time"] = 0;
        /**/

        JsonRPCRequest* lReq = rpc.sendRequest("transfer",lObj);
        QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(transferResponse(QJsonObject,QJsonObject)));
    }

    void store() {
        rpc.sendRequest("store");
    }

    void getPayments(const QString& pPaymentId) {

        QJsonObject lObj;
        lObj.insert("payment_id", pPaymentId);
        rpc.sendRequest("get_payments", lObj);

    }

    void getAddress() {
        JsonRPCRequest* lReq = rpc.sendRequest("getaddress");
        QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(addressResponse(QJsonObject)));
    }


public slots:

    virtual void getBalance() {
        JsonRPCRequest* lReq = rpc.sendRequest("getbalance");
        QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(balanceResponse(QJsonObject)));
    }

    void balanceResponse(const QJsonObject& pObjResponse)
    {
        qDebug() << "Received balance response";
        qDebug() << pObjResponse;

//        if ( pObjResponse["unlocked_balance"].isDouble()) {
//            onBalanceUpdated(pObjResponse["unlocked_balance"].toDouble());
//        }
        if ( pObjResponse["balance"].isDouble()) {
            onBalanceUpdated(pObjResponse["balance"].toDouble());
        }
        else {
            qDebug() << "Format error";
        }
    }

    void transferResponse(const QJsonObject& pObjResponse, const QJsonObject& pObjOriginalParams)
    {
        qDebug() << "Received transfer resposnse";
        qDebug() << pObjResponse;
        qDebug() << "With original params : ";
        qDebug() << pObjOriginalParams;

        if ( pObjResponse["tx_hash"].isString() ) {

            if ( !pObjOriginalParams["destinations"].isArray() ) {
                qDebug() << "WTF no destinations";
                return;
            }

            QJsonArray lDsts = pObjOriginalParams["destinations"].toArray();
            if ( lDsts.empty() ) {
                qDebug() << "WTF empty destinations";
                return;
            }

            const QJsonObject& lDestination = lDsts[0].toObject();
            if ( !lDestination["address"].isString() || !lDestination["amount"].isDouble() ) {
                qDebug() << "WTF destination with bad args";
                return;
            }

            int lOriginalFee = 1000000;
            if ( pObjOriginalParams["fee"].isDouble() ) {
                lOriginalFee = pObjOriginalParams["fee"].toInt();
            }

            onTransferSuccessful(pObjResponse["tx_hash"].toString(), lDestination["amount"].toInt(), lDestination["address"].toString(), lOriginalFee);

//            Ok

        }
        else {
            qDebug() << "Format error";
        }
    }

    void addressResponse(const QJsonObject& pObjResponse)
    {

        if ( pObjResponse["address"].isString() ) {
           this->onAddressUpdated(pObjResponse["address"].toString());
        }
        else {
            qDebug() << "Bad response received for address : " << pObjResponse;
        }

    }

private:
    JsonRPCCommunicator rpc;
};

#endif // RPCWALLET_HH
