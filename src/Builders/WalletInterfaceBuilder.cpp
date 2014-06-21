#include "WalletInterfaceBuilder.h"

//static const std::string avaiable_interfaces[] = {
//    "a",
//    ""
//};
#include "RPC/RPCWallet.h"

WalletInterfaceBuilder::WalletInterfaceBuilder(WalletModel& pModel, const WalletSettings& pSettings)
    : model(pModel), settings(pSettings)
{
}


WalletInterface* WalletInterfaceBuilder::buildInterface() {

    const QString& lInterfaceName = settings.getWalletInterface();

    if (lInterfaceName == "RPC") {
        return new RPCWallet(model,settings);
    }

    throw(std::invalid_argument(lInterfaceName.toStdString() + " : " + "Interface not found."));
//    return NULL;

}
