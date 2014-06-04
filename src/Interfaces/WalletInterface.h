#ifndef WALLETINTERFACE_HH
#define WALLETINTERFACE_HH

#include <QDebug>
#include "Models/WalletModel.h"

class WalletInterface
{
public:
    WalletInterface(WalletModel& pWalletModel) : wallet_model(pWalletModel) {}

    virtual void getBalance() = 0;
    virtual void getAddress() = 0;
    virtual void transfer(double pAmount, const QString& pAddress, int pFee = 1000000, const QString& pPaymentId = "") = 0;
    virtual void getPayments(const QString& pPaymentId) = 0;
    virtual void store() = 0;

protected:

    void onBalanceUpdated(double pBalance, double pUnlockedBalance) {
        wallet_model.setReady(true);
        wallet_model.setBalance(pBalance);
        wallet_model.setUnlockedBalance(pUnlockedBalance);
    }

    void onAddressUpdated(const QString& pAddress) {
        wallet_model.setReady(true);
        wallet_model.setAddress(pAddress);
    }

    void onTransferSuccessful(const QString& pTxHash, double pAmount, const QString& pAddress, int pFee) {
        emit wallet_model.transferSuccessful(pTxHash,pAmount,pAddress,pFee);
        qDebug() << "Transfer Successful : " << pAmount << " to " << pAddress << " (fee : " << pFee << ")\nHash: " << pTxHash;
    }

    void onTransferError(int pErrorCode, const QString& pErrorMessage) {

        qWarning() << "Transfer error " << pErrorCode << " : " << pErrorMessage;

        /* Throws to UI */
        emit wallet_model.transferError(pErrorCode, pErrorMessage);

    }

private:
    WalletModel& wallet_model;
};

#endif // WALLETINTERFACE_HH
