#ifndef MINERINTERFACEBUILDER_H
#define MINERINTERFACEBUILDER_H

#include "WalletSettings.h"
#include "Models/MinerModel.h"

class MinerInterface;

class MinerInterfaceBuilder
{
public:
    MinerInterfaceBuilder(MinerModel &pModel, const WalletSettings& pSettings);

    MinerInterface* buildInterface();

private:
    MinerModel& model;
    const WalletSettings& settings;
};

#endif // MINERINTERFACEBUILDER_H
