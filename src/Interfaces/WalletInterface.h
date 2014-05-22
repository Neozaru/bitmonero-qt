#ifndef WALLETINTERFACE_HH
#define WALLETINTERFACE_HH

#include "Models/WalletModel.h"

class WalletInterface
{
public:
    WalletInterface(WalletModel& pWalletModel) : wallet_model(pWalletModel) {};

    virtual void getBalance() = 0;
    virtual void transfer(double pAmount, const QString& pAddress) = 0;
    virtual void getPayments(const QString& pPaymentId) = 0;
    virtual void store() = 0;

protected:

    virtual void onBalanceUpdated(double pBalance) {
        wallet_model.setBalance(pBalance);
    }



private:
    WalletModel& wallet_model;
};

#endif // WALLETINTERFACE_HH
