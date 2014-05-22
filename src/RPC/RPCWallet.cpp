#include "RPCWallet.h"

RPCWallet::RPCWallet(WalletModel& pModel, const QString& pHost, unsigned int pPort)
    : WalletInterface(pModel), rpc(pHost,pPort)
{

    pModel.setWalletInterface(this);

}
