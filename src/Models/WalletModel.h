#ifndef WALLET_H
#define WALLET_H

#include<QObject>

class WalletInterface;

class WalletModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double balance READ getBalance WRITE setBalance NOTIFY balanceChanged)

public:
    WalletModel();
    ~WalletModel();

    inline void setBalance(double pBalance)
    {

        if ( balance == pBalance ) {
            return;
        }

        balance = pBalance;
//        updated = true;
        emit balanceChanged();

    }


    inline double getBalance() const
    {
        return balance;
    }



    void setWalletInterface(WalletInterface* iface)
    {
        wallet_interface = iface;
    }

//    inline bool isUpdated() const {
//        return updated;
//    }

signals:
    void balanceChanged();

public slots:
    void update() {
        setBalance(99);
    }

    Q_INVOKABLE bool transfer(double amount, const QString& address);


private:
    double balance;
    WalletInterface* wallet_interface;
//    bool updated;
};

#endif // WALLET_H
