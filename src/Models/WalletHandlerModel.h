#ifndef WALLETHANDLERMODEL_H
#define WALLETHANDLERMODEL_H

#include <QObject>



class WalletHandlerInterface;

class WalletHandlerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation WRITE setDefaultWalletLocation NOTIFY defaultWalletLocationChanged)
public:
    WalletHandlerModel();


    Q_INVOKABLE bool createWallet(const QString& pFile, const QString& pPassword);

    Q_INVOKABLE bool tryWalletAsync(const QString& pFile, const QString& pPassword);

    Q_INVOKABLE QList<QObject*>  findWallets(const QString& pPath);


    Q_INVOKABLE bool walletFileExists(const QString &pFile);


    void setWalletHandlerInterface(WalletHandlerInterface* pWalletHandlerInterface) {
        wallet_handler_interface = pWalletHandlerInterface;
    }

    const QString& getDefaultWalletLocation() const
    {
        return default_wallet_location;
    }

public slots:
    void setDefaultWalletLocation(QString pWalletLocation)
    {
        if (default_wallet_location != pWalletLocation) {
            default_wallet_location = pWalletLocation;
            emit defaultWalletLocationChanged(pWalletLocation);
        }
    }

signals:
    void tryWalletResult(bool result);

    void defaultWalletLocationChanged(QString arg);

private:

    WalletHandlerInterface* wallet_handler_interface;

    QString default_wallet_location;
};

#endif // WALLETHANDLERMODEL_H
