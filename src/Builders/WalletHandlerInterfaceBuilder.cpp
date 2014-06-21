#include "WalletHandlerInterfaceBuilder.h"

#include "RPC/WalletHandlerProcess.h"
#include "Interfaces/WalletHandlerInterface.h"


WalletHandlerInterfaceBuilder::WalletHandlerInterfaceBuilder(WalletHandlerModel& pModel, const WalletSettings& pSettings)
    : model(pModel), settings(pSettings)
{
}


WalletHandlerInterface* WalletHandlerInterfaceBuilder::buildInterface() {

    const QString& lInterfaceName = settings.getWalletHandlerInterface();

    if (lInterfaceName == "RPC") {
        return new WalletHandlerProcess(model,settings);
    }

    throw(std::invalid_argument(lInterfaceName.toStdString() + " : " + "Interface not found."));
//    return NULL;

}
