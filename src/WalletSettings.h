#ifndef WALLETSETTINGS_H
#define WALLETSETTINGS_H

#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

#include <QObject>
#include <QSettings>
#include <QDir>

#include <QDebug>

class WalletSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString wallet_password READ getWalletPassword WRITE setWalletPassword)
    Q_PROPERTY(QString wallet_file READ getWalletFile WRITE setWalletFile)

    Q_PROPERTY(QString monero_uri READ getMoneroUri WRITE setMoneroUri)
    Q_PROPERTY(int monero_port READ getMoneroPort WRITE setMoneroPort)
public:

    WalletSettings(const QString& pConfigFile);



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


    const QString& getWalletCliProgram() const {
        return wallet_cli_program;
    }


    void setWalletCliProgram(const QString& pWalletProgram) {
        wallet_cli_program = pWalletProgram;
    }

    const QString& getWalletRpcProgram() const {
        return wallet_rpc_program;
    }


    void setWalletRpcProgram(const QString& pWalletProgram) {
        wallet_rpc_program = pWalletProgram;
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

    int getDaemonKillDelay() const {
        return daemon_kill_delay;
    }


    const QString& getWalletInterface() const {
        return wallet_interface;
    }

    const QString& getWalletHandlerInterface() const {
        return wallet_handler_interface;
    }

    const QString& getMinerInterface() const {
        return miner_interface;
    }


    /* Acceptable : Wallet file and password set OR uses RPC-only wallet */
    Q_INVOKABLE bool areWalletSettingsAcceptable() const {
        return ( !wallet_password.isEmpty() && !wallet_file.isEmpty() ) || !spawn_wallet;
    }

    bool saveWalletConfiguration();
    Q_INVOKABLE bool saveGlobalConfiguration();

    const std::string toString() const {

        std::stringstream lStr;
        lStr << "[Current config]";
        lStr << "\n" << "Daemon : " << getMoneroUri().toStdString() << " " << getMoneroPort();
        lStr << "\n" << "Wallet : " << getWalletUri().toStdString() << " " << getWalletPort();
        lStr << "\n" << "Miner : " << getMinerUri().toStdString() << " " << getMinerPort();
        lStr << "\n" << "Wallet program : " << getWalletCliProgram().toStdString();
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
        qDebug() << "Set Wallet file " << pWalletFile;
        wallet_file = pWalletFile;
    }

    void setMoneroUri(const QString& pMoneroUri)
    {
        qDebug() << "Set Daemon uri : " << pMoneroUri;
        monero_uri = pMoneroUri;
    }

    void setMoneroPort(int pMoneroPort)
    {
        qDebug() << "Set Daemon port : " << QString::number(pMoneroPort);
        monero_port = pMoneroPort;
    }

private:

    QSettings settings;

    QString monero_uri;
    int monero_port;

    QString wallet_uri;
    int wallet_port;

    QString miner_uri;
    int miner_port;

    QString wallet_cli_program;
    QString wallet_rpc_program;
    QString daemon_program;


    bool mining_enabled;
    QString mining_address;


    QString wallet_file;
    QString wallet_password;
    QString wallet_ip;

    bool spawn_wallet;
    bool spawn_daemon;
    int daemon_kill_delay;

    QString wallet_interface;
    QString wallet_handler_interface;
    QString miner_interface;

};

inline std::ostream& operator<<(std::ostream& os, const WalletSettings& pWalletSettings)
{
    os << pWalletSettings.toString();
    return os;
}

#endif // WALLETSETTINGS_H
