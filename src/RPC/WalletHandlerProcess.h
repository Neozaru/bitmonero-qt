#ifndef WALLETHANDLER_H
#define WALLETHANDLER_H

#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QStringListModel>

#include "WalletSettings.h"
#include "Interfaces/WalletHandlerInterface.h"
#include "Models/WalletHandlerModel.h"

/* TODO : Add an abstraction layer */
class WalletHandlerProcess : public WalletHandlerInterface
{

Q_OBJECT

//Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation)
//Q_PROPERTY(const QList<QObject*> last_found_wallets READ getLastFoundWallets NOTIFY lastFoundWalletsChanged)

public:

//    WalletHandlerProcess(const WalletSettings& pWalletSettings);

    WalletHandlerProcess(WalletHandlerModel& pModel, const WalletSettings& pWalletSettings);
    ~WalletHandlerProcess();


    int enable();

//    bool isOk() const {
//        return !main_process.program().isEmpty();
//    }


//    const QString& getDefaultWalletLocation() const
//    {
//        qDebug() << "LOC ";
//        return default_wallet_location;
//    }

    const QList<QObject*>& getLastFoundWallets() const
    {
        return last_found_wallets;
    }

public slots:

    bool createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed);

    bool tryWalletAsync(const QString& pFile, const QString& pPassword);

    QList<QObject*> findWallets(const QString& pPath);


    bool walletFileExists(const QString &pFile);


    bool openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);


private slots:
    void tryWalletResponse(int pCode, QProcess::ExitStatus pExitStatus);


signals:

    void tryWalletResult(bool result);
    void lastFoundWalletsChanged(const QList<QObject*> found_wallets);

private:

    bool closeWallet();

    const WalletSettings& settings;

    QProcess* execTryWallet(const QString& pFile, const QString& pPassword);

    bool walletDirectoryExists(const QString& pFile);

    QProcess main_process;

    QList<QObject*> last_found_wallets;
};

#endif // WALLETHANDLER_H
