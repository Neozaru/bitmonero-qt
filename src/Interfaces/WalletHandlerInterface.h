#ifndef WALLETHANDLERINTERFACE_H
#define WALLETHANDLERINTERFACE_H

#include <QObject>
#include <QDir>
#include <QDebug>

#include "MetaInterface.h"
#include "Models/WalletHandlerModel.h"

class WalletHandlerInterface : public MetaInterface {

public:

    WalletHandlerInterface(WalletHandlerModel& pModel) : wallet_handler_model(pModel) {
        wallet_handler_model.setWalletHandlerInterface(this);
        wallet_handler_model.setDefaultWalletLocation(QDir::homePath() + "/.bitmonero/");
    }

    virtual ~WalletHandlerInterface() {}

    /**
     * @brief createWallet
     * @param pFile
     * @param pPassword
     * @param pSeed
     * @return 0 if successful, 1 if file problem, 2 if password problem, 3 if seed problem, -1 otherwise.
     */
    virtual int createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed) = 0;

    virtual void tryWalletAsync(const QString& pFile, const QString& pPassword) = 0;

    virtual bool walletFileExists(const QString& pFile) = 0;

    virtual QList<QObject*> findWallets(const QString& pPath) = 0;


    /**
     * @brief onTryWalletResult
     * @param pResult 0 if successful, 1 is wallet file error, 2 if password error, -1 otherwise
     */
    void onTryWalletResult(int pResult) {
        emit wallet_handler_model.tryWalletResult(pResult);
    }

    void onSeedGenerated(const QString& pSeed) {
        wallet_handler_model.setSeed(pSeed);
    }

    void setSeedAvailable() {
        wallet_handler_model.setSeedAvailable(true);
    }



private:
    WalletHandlerModel& wallet_handler_model;
};

#endif // WALLETHANDLERINTERFACE_H
