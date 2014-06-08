#include "RPCWallet.h"

#include <QTimer>
#include <iostream>
#include <QObject>

#include "Models/TransactionModel.h"

RPCWallet::RPCWallet(WalletModel& pModel, const WalletSettings& pSettings)
    : WalletInterface(pModel), rpc(pSettings.getWalletUri(), pSettings.getWalletPort()), ready(false)
{

    pModel.setWalletInterface(this);


}

void RPCWallet::enable() {

    getAddress();
    getBalance();

//    QTimer* lBalanceAddressTimer = new QTimer(this);
    QObject::connect(&getbalance_timer,SIGNAL(timeout()), this, SLOT(getBalance()));
    getbalance_timer.start(5000);

    /* TODO: Remove this when address is retrieved */
    QObject::connect(&getaddress_timer,SIGNAL(timeout()), this, SLOT(getAddress()));
    getaddress_timer.start(5000);


//    QTimer* lGetIncomingTransfersTimer = new QTimer(this);
    QObject::connect(&incomingtransfers_timer,SIGNAL(timeout()), this, SLOT(getIncomingTransfers()));

    /* TODO : Change interval to 15-30 sec */
    incomingtransfers_timer.start(3000);

}


void RPCWallet::transfer(double pAmount, const QString& pAddress, int pFee, const QString& pPaymentId) {
    QJsonObject lParams;
    QJsonArray lDests;

    /* Single Dest for the moment */
    QJsonObject lDst;
    lDst["amount"] = pAmount;
    lDst["address"] = pAddress;
    lDests.append(lDst);
    lParams["destinations"] = lDests;
    /**/

    lParams["fee"] = pFee;
    lParams["payment_id"] = pPaymentId;

    /* TODO */
    lParams["mixin"] = 0;
    lParams["unlock_time"] = 0;
    /**/

    JsonRPCRequest* lReq = rpc.sendRequest("transfer",lParams);
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(transferResponse(QJsonObject,QJsonObject)));
}


void RPCWallet::store() {
    rpc.sendRequest("store");
}


void RPCWallet::getPayments(const QString& pPaymentId) {

    QJsonObject lParams;
    lParams.insert("payment_id", pPaymentId);
    rpc.sendRequest("get_payments", lParams);

}


void RPCWallet::getAddress() {
    JsonRPCRequest* lReq = rpc.sendRequest("getaddress");
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(addressResponse(QJsonObject)));
}


void RPCWallet::getBalance() {
    JsonRPCRequest* lReq = rpc.sendRequest("getbalance");
    QObject::connect(lReq,SIGNAL(jsonResponseReceived(QJsonObject,QJsonObject)),this,SLOT(balanceResponse(QJsonObject)));
}

void RPCWallet::getIncomingTransfers(const QString& pType) {

    QJsonObject lParams;
    lParams["transfer_type"] = pType;
    JsonRPCRequest* lReq = rpc.sendRequest("incoming_transfers", lParams);

    QObject::connect(lReq, &JsonRPCRequest::jsonResponseReceived, [this] (const QJsonObject& pJsonResponse) {

        if ( !pJsonResponse["transfers"].isArray() ) {
            return;
        }

        QList<QObject*> lNewTransactions;
        QJsonArray lTransfersJson = pJsonResponse["transfers"].toArray();
        for ( const QJsonValueRef& lTransJsonRef : lTransfersJson ) {

            const QJsonObject lTransJson = lTransJsonRef.toObject();
            lNewTransactions.append(new TransactionModel(lTransJson["tx_hash"].toString(),lTransJson["amount"].toDouble(),lTransJson["spent"].toBool(), false));
        }

        this->onIncomingTransfersUpdated(lNewTransactions);

    });

}




void RPCWallet::transferResponse(const QJsonObject& pObjResponse, const QJsonObject& pObjOriginalParams)
{
    qDebug() << "Received transfer resposnse";
    qDebug() << pObjResponse;
    qDebug() << "With original params : ";
    qDebug() << pObjOriginalParams;

    /* Error */
    if ( pObjResponse["error"].isObject() ) {

        const QJsonObject lError = pObjResponse["error"].toObject();
        int lErrorCode = -1;
        QString lErrorMessage;

        if ( lError.contains("message") ) {
            lErrorMessage = lError["message"].toString();
        }

        if ( lError.contains("code") ) {
            lErrorCode = lError["code"].toInt();
        }

        this->onTransferError(lErrorCode, lErrorMessage);
        return;
    }


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

    getaddress_timer.setInterval(120000);

    if ( pObjResponse["address"].isString() ) {
       this->onAddressUpdated(pObjResponse["address"].toString());
    }
    else {
        qDebug() << "Bad response received for address : " << pObjResponse;
    }

}

void RPCWallet::balanceResponse(const QJsonObject& pObjResponse)
{

    if ( !ready ) {
        ready = true;
        qWarning() << "Balance UPDATED";
        qWarning() << "WALLET READY !!";
        this->onReady();

    }

    if ( pObjResponse["balance"].isDouble() && pObjResponse["unlocked_balance"].isDouble() ) {
        onBalanceUpdated(pObjResponse["balance"].toDouble(), pObjResponse["unlocked_balance"].toDouble());
    }
    else {
        qWarning() << "'get_balance' failed. Is RPC Wallet reachable ?";
    }
}
