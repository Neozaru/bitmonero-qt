#ifndef WALLETSETTINGS_H
#define WALLETSETTINGS_H

#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

#include <QObject>
#include <QSettings>
#include <QDir>

class WalletSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString wallet_password READ getWalletPassword WRITE setWalletPassword);
    Q_PROPERTY(QString wallet_file READ getWalletFile WRITE setWalletFile);

public:

    WalletSettings(const QString& pConfigFile = QDir::homePath() + "/.bitmonero-qt/bitmonero-qt.conf");



    const QSettings& getSettings() const {
        return settings;
    }


    const QString& getMoneroUri() const {
        return monero_uri;
    }


    int getMoneroPort() const {
        return monero_port;
    }


    const QString& getWalletUri() const {
        return wallet_uri;
    }

    int getWalletPort() const {
        return wallet_port;
    }

    const QString& getMinerUri() const {
        return miner_uri;
    }

    int getMinerPort() const {
        return miner_port;
    }


    const QString& getWalletProgram() const {
        return wallet_program;
    }


    void setWalletProgram(const QString& pWalletProgram) {
        wallet_program = pWalletProgram;
    }

    const QString& getDaemonProgram() const {
        return daemon_program;
    }

    void setDaemonProgram(const QString& pDaemonProgram) {
        daemon_program = pDaemonProgram;
    }

    bool isMiningEnabled() {
        return mining_enabled;
    }

    const QString& getMiningAddress() const {
        return mining_address;
    }

    bool isMiningAddressConfigured() const {
        return !mining_address.isEmpty();
    }


    const QString& getWalletFile() const {
        return wallet_file;
    }

    const QString& getWalletPassword() const {
        return wallet_password;
    }

    const QString getWalletIP() const {
        return wallet_ip;
    }

    bool isWalletPasswordDefined() const {
        return !getWalletPassword().isEmpty();
    }


    bool shouldSpawnWallet() const {
        return spawn_wallet;
    }

    bool shouldSpawnDaemon() const {
        return spawn_daemon;
    }

    /* Acceptable : Wallet file and password set OR uses RPC-only wallet */
    Q_INVOKABLE bool areSettingsAcceptable() const {
        return ( !wallet_password.isEmpty() && !wallet_file.isEmpty() ) || !spawn_wallet;
    }

    bool saveWalletConfiguration();

    const std::string toString() const {

        std::stringstream lStr;
        lStr << "[Current config]";
        lStr << "\n" << "Daemon : " << getMoneroUri().toStdString() << " " << getMoneroPort();
        lStr << "\n" << "Wallet : " << getWalletUri().toStdString() << " " << getWalletPort();
        lStr << "\n" << "Miner : " << getMinerUri().toStdString() << " " << getMinerPort();
        lStr << "\n" << "Wallet program : " << getWalletProgram().toStdString();
        lStr << "\n" << "Wallet file : " << getWalletFile().toStdString();

        return lStr.str();

    }


public slots:
    void setWalletPassword(const QString& pWalletPassword)
    {
        wallet_password = pWalletPassword;
    }

    void setWalletFile(const QString& pWalletFile)
    {
        std::cout << "Set Wallet file " << pWalletFile.toStdString() << std::endl;
        wallet_file = pWalletFile;
    }

private:

    QSettings settings;

    QString monero_uri;
    int monero_port;

    QString wallet_uri;
    int wallet_port;

    QString miner_uri;
    int miner_port;

    QString wallet_program;
    QString daemon_program;

    bool mining_enabled;
    QString mining_address;


    QString wallet_file;
    QString wallet_password;
    QString wallet_ip;

    bool spawn_wallet;
    bool spawn_daemon;

};

inline std::ostream& operator<<(std::ostream& os, const WalletSettings& pWalletSettings)
{
    os << pWalletSettings.toString();
    return os;
}

#endif // WALLETSETTINGS_H
