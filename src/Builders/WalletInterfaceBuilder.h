#ifndef WALLETINTERFACEBUILDER_H
#define WALLETINTERFACEBUILDER_H

#include "WalletSettings.h"
#include "Models/WalletModel.h"
#include "Interfaces/MoneroInterface.h"

class WalletInterface;

class WalletInterfaceBuilder
{
public:
    WalletInterfaceBuilder(WalletModel &pModel, const WalletSettings& pSettings, const MoneroInterface& pMoneroInterface);

    WalletInterface* buildInterface();

private:
    WalletModel& model;
    const WalletSettings& settings;
    const MoneroInterface& monero_interface;
};

#endif // WALLETINTERFACEBUILDER_H
