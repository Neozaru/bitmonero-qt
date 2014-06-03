#include "RPCWallet.h"

#include <QTimer>
#include <iostream>


RPCWallet::RPCWallet(WalletModel& pModel, const WalletSettings& pSettings)
    : WalletInterface(pModel), rpc(pSettings.getWalletUri(), pSettings.getWalletPort())
{

    pModel.setWalletInterface(this);
    getAddress();
    getBalance();

    QTimer* lTimer = new QTimer(this);
    QObject::connect(lTimer,SIGNAL(timeout()), this, SLOT(getBalance()));
    QObject::connect(lTimer,SIGNAL(timeout()), this, SLOT(getAddress()));

    lTimer->start(5000);


}



void RPCWallet::transfer(double pAmount, const QString& pAddress, int pFee, const QString& pPaymentId) {
    QJsonObject lObj;
    QJsonArray lDests;

    /* Single Dest for the moment */
    QJsonObject lDst;
    lDst["amount"] = pAmount;
    lDst["address"] = pAddress;
    lDests.append(lDst);
    lObj["destinations"] = lDests;
    /**/

    lObj["fee"] = pFee;
    lObj["payment_id"] = pPaymentId;

    /* TODO */
    lObj["mixin"] = 0;
    lObj["unlock_time"] = 0;
    /**/

    JsonRPCRequest* lReq = rpc.sendRequest("transfer",lObj);
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(transferResponse(QJsonObject,QJsonObject)));
}


void RPCWallet::store() {
    rpc.sendRequest("store");
}


void RPCWallet::getPayments(const QString& pPaymentId) {

    QJsonObject lObj;
    lObj.insert("payment_id", pPaymentId);
    rpc.sendRequest("get_payments", lObj);

}


void RPCWallet::getAddress() {
    JsonRPCRequest* lReq = rpc.sendRequest("getaddress");
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(addressResponse(QJsonObject)));
}


void RPCWallet::getBalance() {
    JsonRPCRequest* lReq = rpc.sendRequest("getbalance");
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(balanceResponse(QJsonObject)));
}


void RPCWallet::balanceResponse(const QJsonObject& pObjResponse)
{

//        if ( pObjResponse["unlocked_balance"].isDouble()) {
//            onBalanceUpdated(pObjResponse["unlocked_balance"].toDouble());
//        }
    if ( pObjResponse["balance"].isDouble()) {
        onBalanceUpdated(pObjResponse["balance"].toDouble());
    }
    else {
        qWarning() << "'get_balance' failed. Is RPC Wallet reachable ?";
    }
}


void RPCWallet::transferResponse(const QJsonObject& pObjResponse, const QJsonObject& pObjOriginalParams)
{
    qDebug() << "Received transfer resposnse";
    qDebug() << pObjResponse;
    qDebug() << "With original params : ";
    qDebug() << pObjOriginalParams;

    if ( !pObjResponse["tx_hash"].isString() ) {
        qWarning() << "Bad JSON response for transfer : " << pObjResponse;
        return;
    }

    /* TOOD : Create a serialization:deserialization function for this stuff */

    if ( !pObjOriginalParams["destinations"].isArray() ) {
        qCritical() << "transferResponse brought bad original params : " << pObjOriginalParams;
        return;
    }


    QJsonArray lDsts = pObjOriginalParams["destinations"].toArray();
    const QJsonObject& lDestination = lDsts.first().toObject();
    if ( !lDestination["address"].isString() || !lDestination["amount"].isDouble() ) {
        qCritical() << "transferResponse brought bad original params : " << pObjOriginalParams;
        return;
    }

    unsigned int lOriginalFee = 1000000;
    if ( pObjOriginalParams["fee"].isDouble() ) {
        lOriginalFee = pObjOriginalParams["fee"].toInt();
    }

    /* OK */
    onTransferSuccessful(pObjResponse["tx_hash"].toString(), lDestination["amount"].toInt(), lDestination["address"].toString(), lOriginalFee);

}


void RPCWallet::addressResponse(const QJsonObject& pObjResponse)
{

    if ( pObjResponse["address"].isString() ) {
       this->onAddressUpdated(pObjResponse["address"].toString());
    }
    else {
        qDebug() << "Bad response received for address : " << pObjResponse;
    }

}
