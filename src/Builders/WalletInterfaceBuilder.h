#ifndef WALLETINTERFACEBUILDER_H
#define WALLETINTERFACEBUILDER_H

#include "WalletSettings.h"
#include "Models/WalletModel.h"

class WalletInterface;

class WalletInterfaceBuilder
{
public:
    WalletInterfaceBuilder(WalletModel &pModel, const WalletSettings& pSettings);

    WalletInterface* buildInterface();

private:
    WalletModel& model;
    const WalletSettings& settings;
};

#endif // WALLETINTERFACEBUILDER_H
