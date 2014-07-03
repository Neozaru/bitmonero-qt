#ifndef WALLETHANDLERMODEL_H
#define WALLETHANDLERMODEL_H

#include <QObject>



class WalletHandlerInterface;

class WalletHandlerModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation WRITE setDefaultWalletLocation NOTIFY defaultWalletLocationChanged)
    Q_PROPERTY(QString ephemeral_seed READ getSeed WRITE setSeed NOTIFY seedChanged)
    Q_PROPERTY(bool seed_available READ isSeedAvailable WRITE setSeedAvailable NOTIFY seedAvailableChanged)
public:
    WalletHandlerModel();


    Q_INVOKABLE int createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed);

    Q_INVOKABLE void tryWalletAsync(const QString& pFile, const QString& pPassword);

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

    bool isSeedAvailable() const
    {
        return seed_available;
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

    void setSeedAvailable(bool pSeedAvailable)
    {
        if (seed_available != pSeedAvailable) {
            seed_available = pSeedAvailable;
            emit seedAvailableChanged(pSeedAvailable);
        }
    }

signals:
    void tryWalletResult(int result);

    void defaultWalletLocationChanged(QString arg);

    void seedChanged(QString seed);

    void seedAvailableChanged(bool arg);

private:

    WalletHandlerInterface* wallet_handler_interface;

    QString default_wallet_location;
    QString ephemeral_seed;
    bool seed_available;
};

#endif // WALLETHANDLERMODEL_H
