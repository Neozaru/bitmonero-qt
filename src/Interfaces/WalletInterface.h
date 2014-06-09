#ifndef WALLETINTERFACE_HH
#define WALLETINTERFACE_HH

#include <QDebug>
#include <QList>

#include "Models/WalletModel.h"
#include "Models/TransactionModel.h"

class WalletInterface : public QObject
{
    Q_OBJECT
public:
    WalletInterface(WalletModel& pWalletModel) : wallet_model(pWalletModel), wallet_ready(false) {}

    virtual void getBalance() = 0;
    virtual void getAddress() = 0;
    virtual void transfer(double pAmount, const QString& pAddress, int pFee = 1000000, const QString& pPaymentId = "") = 0;
    virtual void getPayments(const QString& pPaymentId) = 0;
    virtual void store() = 0;

    virtual int enable() = 0;

//    virtual bool openWalletAsync();
    virtual bool isOk() = 0;

signals:
    /* TODO : Check if still useful */
    void ready();

//    void walletOpened();



protected:

    void onBalanceUpdated(double pBalance, double pUnlockedBalance)
    {
        wallet_model.setReady(true);
        wallet_model.setBalance(pBalance);
        wallet_model.setUnlockedBalance(pUnlockedBalance);
    }


    void onAddressUpdated(const QString& pAddress)
    {
        wallet_model.setReady(true);
        wallet_model.setAddress(pAddress);
    }


    void onTransferSuccessful(const QString& pTxHash, double pAmount, const QString& pAddress, int pFee)
    {
        emit wallet_model.transferSuccessful(pTxHash,pAmount,pAddress,pFee);
        qDebug() << "Transfer Successful : " << pAmount << " to " << pAddress << " (fee : " << pFee << ")\nHash: " << pTxHash;
    }


    void onIncomingTransfersUpdated(const QList<QObject*>& pTransfers) {
        wallet_model.setTransactions(pTransfers);
        qDebug() << "Transactions SET";
    }


    void onTransferError(int pErrorCode, const QString& pErrorMessage) {

        qWarning() << "Transfer error " << pErrorCode << " : " << pErrorMessage;

        /* Throws to UI */
        emit wallet_model.transferError(pErrorCode, pErrorMessage);

    }

    void onReady() {

        if ( !wallet_ready ) {
            wallet_ready = true;
            emit ready();
        }

    }

//signals:
//    void walletReady();

private:
    WalletModel& wallet_model;

    bool wallet_ready;
};

#endif // WALLETINTERFACE_HH
