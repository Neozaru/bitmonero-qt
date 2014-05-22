#include "WalletModel.h"


#include "Interfaces/WalletInterface.h"

#include <QDebug>

WalletModel::WalletModel()
    : balance(0)
{
}

WalletModel::~WalletModel()
{

}



bool WalletModel::transfer(double amount, const QString& address)
{
    qDebug() << amount << " to " << address;

    if ( address.size() != 95 || amount != amount || amount <= 0 ) {
        return false;
    }

    wallet_interface->transfer(amount,address);

    return true;

}
