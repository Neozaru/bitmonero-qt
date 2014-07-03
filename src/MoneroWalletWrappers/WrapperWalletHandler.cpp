#include "WrapperWalletHandler.h"

#include "MoneroWallet/MoneroWallet.hh"
#include "Utils.h"

#include <QtConcurrent/QtConcurrent>

WrapperWalletHandler::WrapperWalletHandler(WalletHandlerModel& pModel, const WalletSettings& pSettings)
    : WalletHandlerInterface(pModel), settings(pSettings)
{
    this->setSeedAvailable();
}


void WrapperWalletHandler::enable() {

    emit onReady();
}

int WrapperWalletHandler::createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed) {

    if (pPassword.isEmpty()) {
        return 2;
    }

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

        return 0;
    }
    catch(Monero::Errors::NotWritableFile e) {
        qWarning() << "Exception : Not writable file";
        return 1;
    }
    catch(Monero::Errors::InvalidSeed e) {
        qWarning() << "Exception : Invalid seed" << "(" << pSeed << ")";
        return 3;
    }
}

void WrapperWalletHandler::tryWalletAsync(const QString& pFile, const QString& pPassword) {

    try {
        Monero::Wallet* lWallet = new Monero::Wallet(pFile.toStdString(), pPassword.toStdString());

        QtConcurrent::run([this, lWallet]() {

            const QString lMoneroAddressPort = settings.getMoneroUri() + ":" + QString::number(settings.getMoneroPort());

            lWallet->connect(lMoneroAddressPort.toStdString());

            this->onTryWalletResult( lWallet->refresh() ? 0 : -1 );
            lWallet->store();
            delete lWallet;
        });

    }
    catch(Monero::Errors::InvalidFile e) {
        this->onTryWalletResult(1);
    }
    catch(Monero::Errors::InvalidPassword e) {
        this->onTryWalletResult(2);
    }
    catch(const std::exception& e) {
        this->onTryWalletResult(-1);
    }

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
