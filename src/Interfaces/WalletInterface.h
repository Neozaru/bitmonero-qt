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
    virtual void transfer(double pAmount, const QString& pAddress, int pFee = 1000000) = 0;
    virtual void getPayments(const QString& pPaymentId) = 0;
    virtual void store() = 0;

protected:

    void onBalanceUpdated(double pBalance) {
        wallet_model.setBalance(pBalance);
    }

    void onAddressUpdated(const QString& pAddress) {
        wallet_model.setAddress(pAddress);
    }

    void onTransferSuccessful(const QString& pTxHash, double pAmount, const QString& pAddress, int pFee) {
        qDebug() << "SUCCESSFUL : " << pAmount << " to " << pAddress << " (fee : " << pFee << ")\nHash: " << pTxHash;
        emit wallet_model.transferSuccessful(pTxHash,pAmount,pAddress,pFee);
//        wallet_model.
    }

private:
    WalletModel& wallet_model;
};

#endif // WALLETINTERFACE_HH
