#ifndef WALLETHANDLER_H
#define WALLETHANDLER_H

#include <QProcess>
#include <QObject>
#include <QDebug>
#include <QStringListModel>

#include "WalletSettings.h"

/* TODO : Add an abstraction layer */
class WalletHandler : public QObject
{

Q_OBJECT

Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation)
Q_PROPERTY(const QList<QObject*> last_found_wallets READ getLastFoundWallets NOTIFY lastFoundWalletsChanged)

public:

    WalletHandler(const WalletSettings& pWalletSettings);
    ~WalletHandler();

    bool isOk() const {
        return !main_process.program().isEmpty();
    }


    const QString& getDefaultWalletLocation() const
    {
        qDebug() << "LOC ";
        return default_wallet_location;
    }

    const QList<QObject*>& getLastFoundWallets() const
    {
        return last_found_wallets;
    }

public slots:

    Q_INVOKABLE bool createWallet(const QString& pFile, const QString& pPassword);

    Q_INVOKABLE bool tryWalletAsync(const QString& pFile, const QString& pPassword);

    Q_INVOKABLE bool findWallets(const QString& pPath = "");


    bool openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);


    Q_INVOKABLE bool walletFileExists(const QString &pFile);


    bool isOpen() const {
        return open;
    }


private slots:
    void tryWalletResponse(int pCode, QProcess::ExitStatus pExitStatus);

signals:
    void tryWalletResult(bool result);

    void lastFoundWalletsChanged(const QList<QObject*> found_wallets);

private:

    bool closeWallet();

    QProcess* execTryWallet(const QString& pFile, const QString& pPassword);

    bool walletDirectoryExists(const QString& pFile);

    bool open;
    QString default_wallet_location;
    QProcess main_process;

    QList<QObject*> last_found_wallets;
};

#endif // WALLETHANDLER_H
