#ifndef WALLET_H
#define WALLET_H

#include<QObject>
#include <QDebug>

class WalletInterface;

class WalletModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double balance READ getBalance WRITE setBalance NOTIFY balanceChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY addressChanged)

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

    void setAddress(const QString& pAddress) {
        if ( pAddress != address ) {

            if ( pAddress.length() != 95 ) {
                qDebug() << "Attempted to assign bad address : " << pAddress;
                return;
            }

            qDebug() << "Wallet address changed" << pAddress;
            address = pAddress;
            emit addressChanged(address);

        }
    }

    QString getAddress() const
    {
        return address;
    }

signals:
    void balanceChanged();
    void transferSuccessful(const QString& tx_hash, double amount, const QString& address, int fee);
    void addressChanged(const QString& pAddress);

public slots:
    void update() {
        setBalance(99);
    }

    Q_INVOKABLE bool transfer(double amount, const QString& address);


private:
    double balance;
    WalletInterface* wallet_interface;
    //    bool updated;
    QString address;
};

#endif // WALLET_H
