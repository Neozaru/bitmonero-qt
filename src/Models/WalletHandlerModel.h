#ifndef WALLETHANDLERMODEL_H
#define WALLETHANDLERMODEL_H

#include <QObject>



class WalletHandlerInterface;

class WalletHandlerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation WRITE setDefaultWalletLocation NOTIFY defaultWalletLocationChanged)
    Q_PROPERTY(QString ephemeral_seed READ getSeed WRITE setSeed NOTIFY seedChanged)
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

    QString getSeed() const
    {
        return ephemeral_seed;
    }

public slots:
    void setDefaultWalletLocation(QString pWalletLocation)
    {
        if (default_wallet_location != pWalletLocation) {
            default_wallet_location = pWalletLocation;
            emit defaultWalletLocationChanged(pWalletLocation);
        }
    }

    void setSeed(QString pSeed)
    {
        if (ephemeral_seed != pSeed) {
            ephemeral_seed = pSeed;
            emit seedChanged(pSeed);
        }
    }

signals:
    void tryWalletResult(bool result);

    void defaultWalletLocationChanged(QString arg);

    void seedChanged(QString seed);

private:

    WalletHandlerInterface* wallet_handler_interface;

    QString default_wallet_location;
    QString ephemeral_seed;
};

#endif // WALLETHANDLERMODEL_H
