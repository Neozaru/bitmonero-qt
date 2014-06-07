#ifndef WALLETHANDLER_H
#define WALLETHANDLER_H

#include <QProcess>
#include <QObject>
#include <QDebug>

#include "WalletSettings.h"

/* TODO : Add an abstraction layer */
class WalletHandler : public QObject
{

Q_OBJECT

Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation)


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

public slots:

    Q_INVOKABLE bool createWallet(const QString& pFile, const QString& pPassword);

    bool tryWalletProgram();


    bool tryWallet(const QString& pFile, const QString& pPassword);
    Q_INVOKABLE bool tryWalletAsync(const QString& pFile, const QString& pPassword);

    bool closeWallet();
    bool openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);

    bool isOpen() const {
        return open;
    }

    Q_INVOKABLE bool walletFileExists(const QString &pFile);

private slots:
    void tryWalletResponse(int pCode, QProcess::ExitStatus pExitStatus);

signals:
    void tryWalletResult(bool result);

private:

    QProcess* execTryWallet(const QString& pFile, const QString& pPassword);

    bool walletDirectoryExists(const QString& pFile);

    bool open;
    QString default_wallet_location;
    QProcess main_process;

};

#endif // WALLETHANDLER_H
