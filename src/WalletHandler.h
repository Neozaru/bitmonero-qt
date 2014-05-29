#ifndef WALLETHANDLER_H
#define WALLETHANDLER_H

#include <QProcess>


class WalletHandler
{
public:
    WalletHandler(const QString& pProgramPath);

    bool createWallet(const QString& pFile, const QString& pPassword);


    bool closeWallet();
    bool openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);

    bool isOpen() const {
        return open;
    }

private:

    QProcess mainProcess;
    bool open;
};

#endif // WALLETHANDLER_H
