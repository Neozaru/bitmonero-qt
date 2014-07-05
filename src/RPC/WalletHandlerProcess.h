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

    /**
     * @brief Lanch 'simplewallet' subprocess in "--command" mode, causing exiting with code 0 if file/password/seeds are correct
     * Triggers 'tryWalletResult' signal
     * @param pFile
     * @param pPassword
     */
    void tryWalletAsync(const QString& pFile, const QString& pPassword);

    QList<QObject*> findWallets(const QString& pPath);


    bool walletFileExists(const QString &pFile);

    /**
     * @brief Opens a wallet using 'simplewallet' subprocess, and enables RPC.
     * After starting the subprocess, the function will block x (=3) seconds to check if the program exists abruptly.
     * If program is not stopped after the delay, we assume that it was successfully opened in RPC mode. (infinite run)
     * @param pWalletFile
     * @param pWalletPassword
     * @param pBindIP RPC wallet host (localhost)
     * @param pBindPort RPC wallet port (19091)
     * @return false in process exits before xx seconds (error), true otherwise
     */
    bool openWallet(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);


private slots:
    void tryWalletResponse(int pCode, QProcess::ExitStatus pExitStatus);
    void tryWalletError(QProcess::ProcessError pError);


signals:
    void tryWalletResult(int result);
    void lastFoundWalletsChanged(const QList<QObject*> found_wallets);

private:

    bool closeWallet();
    bool walletDirectoryExists(const QString& pFile);

    QProcess* createTryWalletProcess(const QString& pFile, const QString& pPassword);


    const WalletSettings& settings;
    QString wallet_cli_program;
    QString wallet_rpc_program;

    QProcess main_process;
    QList<QObject*> last_found_wallets;
};

#endif // WALLETHANDLER_H
