#include "WalletHandlerModel.h"

#include "Interfaces/WalletHandlerInterface.h"

WalletHandlerModel::WalletHandlerModel()
    : wallet_handler_interface(NULL)
{
}



int WalletHandlerModel::createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed)
{
    return wallet_handler_interface->createWallet(pFile, pPassword, pSeed);
}

void WalletHandlerModel::tryWalletAsync(const QString& pFile, const QString& pPassword)
{
    wallet_handler_interface->tryWalletAsync(pFile, pPassword);
}

QList<QObject*> WalletHandlerModel::findWallets(const QString& pPath)
{
    return wallet_handler_interface->findWallets(pPath);
}


bool WalletHandlerModel::walletFileExists(const QString &pFile)
{
    return wallet_handler_interface->walletFileExists(pFile);
}
