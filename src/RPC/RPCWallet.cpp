#include "RPCWallet.h"

#include <QTimer>
#include <iostream>
#include <QObject>
#include <QtCore/qmath.h>

#include "Models/TransactionModel.h"

RPCWallet::RPCWallet(WalletModel& pModel, const WalletSettings& pSettings)
    : WalletInterface(pModel), wallet_handler(pSettings), settings(pSettings),ready(false), rpc(pSettings.getWalletUri(), pSettings.getWalletPort())
{

    pModel.setWalletInterface(this);
    should_spawn_wallet = pSettings.shouldSpawnWallet();


}

RPCWallet::~RPCWallet() {

    getbalance_timer.stop();
    getaddress_timer.stop();
    incomingtransfers_timer.stop();

}

int RPCWallet::enable() {

    if ( should_spawn_wallet ) {

        if ( !wallet_handler.isOk() ) {
            return 1;
        }

        /* TODO : Emit errors */
        if ( !wallet_handler.tryWalletAsync(settings.getWalletFile(), settings.getWalletPassword()) ) {
            qWarning() << "Wallet opening failed. Aborting.";
    //        exit_status = 2;
            return 2;
        }

        QObject::connect(&wallet_handler, &WalletHandler::tryWalletResult, [this] (bool pResult) {

            qDebug() << "[OK] Wallet try";
            qDebug() << "With result : " << pResult;
            if (!pResult) {
                qDebug() << "Simplewallet try failed. Aborting.";
    //            exit_status = 3;
    //            emit applicationQuit(3);
    //            return 3;
                return 1;
            }

            if ( !wallet_handler.openWalletAsync(settings.getWalletFile(), settings.getWalletPassword(), settings.getWalletIP(), settings.getWalletPort()) ) {
                qDebug() << "Failed to start wallet ("<< settings.getWalletProgram() << ")";
    //            exit_status = 4;
    //            emit applicationQuit(4);
    //            return 4;
            }

            return 0;

        });

    }

    QObject::connect(&getbalance_timer,SIGNAL(timeout()), this, SLOT(getBalance()));
    getbalance_timer.start(5000);

    QObject::connect(&getaddress_timer,SIGNAL(timeout()), this, SLOT(getAddress()));
    getaddress_timer.start(5000);


    QObject::connect(&incomingtransfers_timer,SIGNAL(timeout()), this, SLOT(getIncomingTransfers()));
    incomingtransfers_timer.start(30000);


    return 0;

}


void RPCWallet::transfer(unsigned long long pAmount, const QString& pAddress, unsigned long long pFee, const QString& pPaymentId) {
    QJsonObject lParams;
    QJsonArray lDests;

    /* Single Dest for the moment */
    QJsonObject lDst;
    lDst["amount"] = QJsonValue::fromVariant(QVariant::fromValue(pAmount));
    lDst["address"] = pAddress;
    lDests.append(lDst);
    lParams["destinations"] = lDests;
    /**/

    lParams["fee"] = QJsonValue::fromVariant(QVariant::fromValue(pFee >= 5000000000uLL ? pFee : 5000000000uLL));
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

        QList<TransactionModel*> lNewTransactions;
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

    unsigned long long lOriginalFee = 5000000000uLL;
    if ( pObjOriginalParams["fee"].isDouble() ) {
        lOriginalFee = pObjOriginalParams["fee"].toDouble();
    }

    /* OK */
    onTransferSuccessful(pObjResponse["tx_hash"].toString(), lDestination["amount"].toDouble(), lDestination["address"].toString(), lOriginalFee);

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
