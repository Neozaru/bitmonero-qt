#include "WalletInterfaceBuilder.h"

#include "RPC/RPCWallet.h"

#ifdef LIBMONEROWALLET_ENABLED
#include "MoneroWalletWrappers/WrapperWallet.h"
#endif

WalletInterfaceBuilder::WalletInterfaceBuilder(WalletModel& pModel, const WalletSettings& pSettings)
    : model(pModel), settings(pSettings)
{
}


WalletInterface* WalletInterfaceBuilder::buildInterface() {

    const QString& lInterfaceName = settings.getWalletInterface();

    if (lInterfaceName == "RPC") {
        return new RPCWallet(model,settings);
    }

    #ifdef LIBMONEROWALLET_ENABLED
    if (lInterfaceName == "LibMonero") {
        return new WrapperWallet(model, settings);
    }
    #endif

    throw(std::invalid_argument(lInterfaceName.toStdString() + " : " + "Interface not found."));

}
