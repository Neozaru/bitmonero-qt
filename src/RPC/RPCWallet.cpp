#include "RPCWallet.h"

#include<QTimer>

RPCWallet::RPCWallet(WalletModel& pModel, const QString& pHost, unsigned int pPort)
    : WalletInterface(pModel), rpc(pHost,pPort)
{

    pModel.setWalletInterface(this);
    getAddress();
    getBalance();

    QTimer* lTimer = new QTimer(this);
    QObject::connect(lTimer,SIGNAL(timeout()), this, SLOT(getBalance()));
    lTimer->start(5000);
//    lTimer->start();

}
