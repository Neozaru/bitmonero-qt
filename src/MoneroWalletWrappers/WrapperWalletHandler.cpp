#include "WrapperWalletHandler.h"

#include "MoneroWallet/MoneroWallet.hh"
#include "Utils.h"

#include <QtConcurrent/QtConcurrent>

WrapperWalletHandler::WrapperWalletHandler(WalletHandlerModel& pModel, const WalletSettings& pSettings)
    : WalletHandlerInterface(pModel), settings(pSettings)
{
    this->setSeedAvailable();
}


int WrapperWalletHandler::enable() {

    emit onReady();
    return 0;
}

bool WrapperWalletHandler::createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed) {

    try {
        /* Check if seed could be placed in a safe memory place ? */
        std::string lSeed;

        if (pSeed.isEmpty()) {
            lSeed = Monero::Wallet::generateWallet(pFile.toStdString(), pPassword.toStdString());
        }
        else {
            lSeed = Monero::Wallet::recoverWallet(pFile.toStdString(), pPassword.toStdString(), pSeed.toStdString());
        }

        /* TODO : Handle non-deterministic wallets ? */
        if (!lSeed.empty()) {
            this->onSeedGenerated(QString::fromStdString(lSeed));
        }

        return true;
    }
    catch(Monero::Errors::NotWritableFile e) {
        qWarning() << "Exception : Not writable file";
        return false;
    }
    catch(Monero::Errors::InvalidSeed e) {
        qWarning() << "Exception : Invalid seed" << "(" << pSeed << ")";
        return false;
    }
}

bool WrapperWalletHandler::tryWalletAsync(const QString& pFile, const QString& pPassword) {

    try {
        Monero::Wallet* lWallet = new Monero::Wallet(pFile.toStdString(), pPassword.toStdString());

        QtConcurrent::run([this, lWallet]() {

            const QString lMoneroAddressPort = settings.getMoneroUri() + ":" + QString::number(settings.getMoneroPort());

            lWallet->connect(lMoneroAddressPort.toStdString());

            this->onTryWalletResult( lWallet->refresh() );
            lWallet->store();
            delete lWallet;
        });

        return true;
    }
    catch(Monero::Errors::InvalidFile e) {
        this->onTryWalletResult(false);
        return false;
    }
    catch(Monero::Errors::InvalidPassword e) {
        this->onTryWalletResult(false);
        return false;
    }
    catch(const std::exception& e) {
        this->onTryWalletResult(false);
        return false;
    }

//    return false;
}

bool WrapperWalletHandler::walletFileExists(const QString& pFile) {

    bool lWalletFileExists;
    bool lWalletKeysExist;
    Monero::Wallet::walletExists(pFile.toStdString(), lWalletFileExists, lWalletKeysExist);

    return lWalletFileExists;

}

QList<QObject*> WrapperWalletHandler::findWallets(const QString& pPath) {

    const QUrl& lUrl = QUrl::fromUserInput(pPath);

    QStringList lWalletsFilesList =  Utils::findWalletsKeysFiles(lUrl);
    return Utils::fileListToInfoWalletModelList(lWalletsFilesList, lUrl);

}
