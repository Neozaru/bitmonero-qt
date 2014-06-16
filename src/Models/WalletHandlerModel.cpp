#include "WalletHandlerModel.h"

#include "Interfaces/WalletHandlerInterface.h"

WalletHandlerModel::WalletHandlerModel()
    : wallet_handler_interface(NULL)
{
}



bool WalletHandlerModel::createWallet(const QString& pFile, const QString& pPassword)
{
    return wallet_handler_interface->createWallet(pFile, pPassword);
}

bool WalletHandlerModel::tryWalletAsync(const QString& pFile, const QString& pPassword)
{
    return wallet_handler_interface->tryWalletAsync(pFile, pPassword);
}

QList<QObject*> WalletHandlerModel::findWallets(const QString& pPath)
{
    return wallet_handler_interface->findWallets(pPath);
}


bool WalletHandlerModel::walletFileExists(const QString &pFile)
{
    return wallet_handler_interface->walletFileExists(pFile);
}
