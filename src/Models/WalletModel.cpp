#include "WalletModel.h"


#include "Interfaces/WalletInterface.h"

#include <QDebug>

WalletModel::WalletModel()
    : wallet_interface(NULL), balance(-1), ready(false)
{
}

WalletModel::~WalletModel()
{

}



bool WalletModel::transfer(double amount, const QString& address, double pFee, const QString& pPaymentId, int pMixinCount)
{
    qDebug() << amount << " to " << address;

    if ( address.size() != 95 || amount != amount || amount <= 0 || pMixinCount < 0 || pMixinCount > 5) {
        qDebug() << "Bad parameter(s)";
        return false;
    }

    wallet_interface->transfer(amount,address,pFee,pPaymentId, pMixinCount);

    return true;
}
