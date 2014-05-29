#ifndef WALLETHANDLER_H
#define WALLETHANDLER_H

#include <QProcess>
#include <QObject>
#include <QDir>
#include <QDebug>

class WalletHandler : public QObject
{

Q_OBJECT

Q_PROPERTY(QString default_wallet_location READ getDefaultWalletLocation)


public:
//    WalletHandler();
    WalletHandler(const QString& pProgramPath = "/usr/bin/simplewallet");

    const QString& getDefaultWalletLocation() const
    {
        qDebug() << "LOC ";
        return default_wallet_location;
    }

public slots:

    Q_INVOKABLE bool createWallet(const QString& pFile, const QString& pPassword);
    bool tryWallet(const QString& pFile, const QString& pPassword);

    bool closeWallet();
    bool openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort);

    bool isOpen() const {
        return open;
    }

private:

    bool walletFileExists(const QString &pFile);
    bool walletDirectoryExists(const QString& pFile);

    QString default_wallet_location = QDir::homePath() + "/.bitmonero/";
    QProcess mainProcess;
    bool open;

};

#endif // WALLETHANDLER_H
