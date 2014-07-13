#include "WalletInterfaceBuilder.h"

#include "RPC/RPCWallet.h"

#ifdef LIBMONEROWALLET_ENABLED
#include "MoneroWalletWrappers/WrapperWallet.h"
#endif

WalletInterfaceBuilder::WalletInterfaceBuilder(WalletModel& pModel, const WalletSettings& pSettings, const MoneroInterface& pMoneroInterface)
    : model(pModel), settings(pSettings), monero_interface(pMoneroInterface)
{
}


WalletInterface* WalletInterfaceBuilder::buildInterface() {

    const QString& lInterfaceName = settings.getWalletInterface();

    if (lInterfaceName == "RPC") {
        return new RPCWallet(model,settings,monero_interface);
    }

    #ifdef LIBMONEROWALLET_ENABLED
    if (lInterfaceName == "LibMonero") {
        return new WrapperWallet(model, settings, monero_interface);
    }
    #endif

    throw(std::invalid_argument(lInterfaceName.toStdString() + " : " + "Interface not found."));

}
