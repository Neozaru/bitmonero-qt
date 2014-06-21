#ifndef WALLETHANDLERINTERFACEBUILDER_H
#define WALLETHANDLERINTERFACEBUILDER_H

#include "Models/WalletHandlerModel.h"
#include "WalletSettings.h"

class WalletHandlerInterface;

class WalletHandlerInterfaceBuilder
{
public:
    WalletHandlerInterfaceBuilder(WalletHandlerModel& pModel, const WalletSettings& pSettings);

    WalletHandlerInterface* buildInterface();

private:
    WalletHandlerModel& model;
    const WalletSettings& settings;
};

#endif // WALLETHANDLERINTERFACEBUILDER_H
