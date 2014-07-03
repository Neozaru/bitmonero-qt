#ifndef WALLETHANDLER_H
#define WALLETHANDLER_H

#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QStringListModel>

#include "WalletSettings.h"
#include "Interfaces/WalletHandlerInterface.h"
#include "Models/WalletHandlerModel.h"

/**
 * @brief The WalletHandlerProcess class
 */
class WalletHandlerProcess : public WalletHandlerInterface
{

Q_OBJECT


public:

    WalletHandlerProcess(WalletHandlerModel& pModel, const WalletSettings& pWalletSettings);
    ~WalletHandlerProcess();

    void enable();


    const QList<QObject*>& getLastFoundWallets() const
    {
        return last_found_wallets;
    }

public slots:

    int createWallet(const QString& pFile, const QString& pPassword, const QString& pSeed);

    void tryWalletAsync(const QString& pFile, const QString& pPassword);

    QList<QObject*> findWallets(const QString& pPath);


    bool walletFileExists(const QString &pFile);

    /**
     * @brief Opens a wallet asynchronously using 'simplewallet' subprocess, enabling RPC.
     * @param pWalletFile
     * @param pWalletPassword
     * @param pBindIP
     * @param pBindPort
     * @return
     */
    bool openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);


private slots:
    void tryWalletResponse(int pCode, QProcess::ExitStatus pExitStatus);
    void tryWalletError(QProcess::ProcessError pError);


signals:

    void tryWalletResult(int result);
    void lastFoundWalletsChanged(const QList<QObject*> found_wallets);

private:

    bool closeWallet();

    const WalletSettings& settings;

    QProcess* createTryWalletProcess(const QString& pFile, const QString& pPassword);

    bool walletDirectoryExists(const QString& pFile);

    QProcess main_process;

    QList<QObject*> last_found_wallets;
};

#endif // WALLETHANDLER_H
