#include "MinerInterfaceBuilder.h"


//static const std::string avaiable_interfaces[] = {
//    "a",
//    ""
//};
#include "RPC/RPCMiner.h"

MinerInterfaceBuilder::MinerInterfaceBuilder(MinerModel& pModel, const WalletSettings& pSettings)
    : model(pModel), settings(pSettings)
{
}


MinerInterface* MinerInterfaceBuilder::buildInterface() {

    const QString& lInterfaceName = settings.getMinerInterface();

    if (lInterfaceName == "RPC") {
        return new RPCMiner(model, settings);
    }

    throw(std::invalid_argument(lInterfaceName.toStdString() + " : " + "Interface not found."));

}
