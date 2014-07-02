#ifndef WRAPPERWALLETHANDLER_H
#define WRAPPERWALLETHANDLER_H

#include "Interfaces/WalletHandlerInterface.h"
#include "Models/WalletHandlerModel.h"
#include "WalletSettings.h"

class WrapperWalletHandler : public WalletHandlerInterface
{
public:
    WrapperWalletHandler(WalletHandlerModel& pModel, const WalletSettings& pSettings);

    void enable();

    int createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed);

    bool tryWalletAsync(const QString& pFile, const QString& pPassword);

    bool walletFileExists(const QString& pFile);

    QList<QObject*> findWallets(const QString& pPath);


private:

    const WalletSettings& settings;
};

#endif // WRAPPERWALLETHANDLER_H
