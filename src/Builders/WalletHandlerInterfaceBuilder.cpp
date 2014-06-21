#include "WalletHandlerInterfaceBuilder.h"

#include "Interfaces/WalletHandlerInterface.h"

#include "RPC/WalletHandlerProcess.h"

#ifdef LIBMONEROWALLET_ENABLED
#include "MoneroWalletWrappers/WrapperWalletHandler.h"
#endif



WalletHandlerInterfaceBuilder::WalletHandlerInterfaceBuilder(WalletHandlerModel& pModel, const WalletSettings& pSettings)
    : model(pModel), settings(pSettings)
{
}


WalletHandlerInterface* WalletHandlerInterfaceBuilder::buildInterface() {

    const QString& lInterfaceName = settings.getWalletHandlerInterface();

    if (lInterfaceName == "RPC") {
        return new WalletHandlerProcess(model,settings);
    }

    #ifdef LIBMONEROWALLET_ENABLED
    if (lInterfaceName == "LibMonero") {
        return new WrapperWalletHandler(model, settings);
    }
    #endif

    throw(std::invalid_argument(lInterfaceName.toStdString() + " : " + "Interface not found."));

}
