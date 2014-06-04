#ifndef WALLET_H
#define WALLET_H

#include <QObject>
#include <QDebug>

#include "AbstractModel.h"

class WalletInterface;

class WalletModel : public AbstractModel
{
    Q_OBJECT
    Q_PROPERTY(double balance READ getBalance WRITE setBalance NOTIFY balanceChanged)
    Q_PROPERTY(double unlocked_balance READ getUnlockedBalance WRITE setUnlockedBalance NOTIFY unlockedBalanceChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY addressChanged)

public:
    WalletModel();
    ~WalletModel();

    void setBalance(double pBalance)
    {

        if ( balance == pBalance ) {
            return;
        }

        balance = pBalance;
        emit balanceChanged();

    }

    double getBalance() const
    {
        return balance;
    }


    void setWalletInterface(WalletInterface* iface)
    {
        wallet_interface = iface;
    }

    void setAddress(const QString& pAddress) {
        if ( pAddress != address ) {

            if ( pAddress.length() != 95 ) {
                qCritical() << "Attempted to assign bad address : " << pAddress;
                return;
            }

            address = pAddress;
            emit addressChanged(address);

        }
    }

    const QString& getAddress() const
    {
        return address;
    }


    void setUnlockedBalance(double pUnlockedBalance)
    {
        if (unlocked_balance != pUnlockedBalance) {
            unlocked_balance = pUnlockedBalance;
            emit unlockedBalanceChanged(pUnlockedBalance);
        }
    }


    double getUnlockedBalance() const
    {
        return unlocked_balance;
    }

signals:
    void balanceChanged();
    void transferSuccessful(const QString& tx_hash, double amount, const QString& address, int fee);
    void addressChanged(const QString& pAddress);
    void transferError(int error_code, const QString& error_message);

    void unlockedBalanceChanged(double arg);

public slots:
    Q_INVOKABLE bool transfer(double amount, const QString& address, int pFee, const QString& pPaymentId);



private:
    WalletInterface* wallet_interface;

    double balance;
    QString address;
    double unlocked_balance;
};

#endif // WALLET_H
