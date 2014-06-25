#ifndef WRAPPERWALLET_H
#define WRAPPERWALLET_H

#include <QTimer>

#include "Interfaces/WalletInterface.h"
#include "WalletSettings.h"


namespace Monero { class Wallet; }

class WrapperWallet : public WalletInterface
{
    Q_OBJECT
public:
    WrapperWallet(WalletModel& pModel, const WalletSettings& pSettings) : WalletInterface(pModel), wallet(NULL), settings(pSettings) {}
     ~WrapperWallet();

    void getBalance();
    void getAddress();
    void transfer(unsigned long long pAmount, const QString& pAddress, unsigned long long pFee, const QString& pPaymentId, int pMixinCount);
    void getPayments(const QString& pPaymentId);
    void getIncomingTransfers(const QString& pFilter = "all");
    void store();

    int enable();


public slots:
    bool refreshWallet();

private:
    Monero::Wallet* wallet;
    const WalletSettings& settings;

    QTimer refresh_timer;
};

#endif // WRAPPERWALLET_H
