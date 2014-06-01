#include "WalletSettings.h"

//WalletSettings::WalletSettings()
//    : WalletSettings(QDir::homePath() + "/.bitmonero-qt/bitmonero-qt.conf")
//{

//}

WalletSettings::WalletSettings(const QString& pConfigFile)
    : settings(pConfigFile, QSettings::IniFormat)
{
    /* Used for auto creating config file */
    if ( !settings.value("mining_enabled").isValid() ) {
        settings.setValue("mining_enabled", false);
    }


    monero_uri = settings.value("daemon_uri", "localhost/json_rpc").toString();
    monero_port = settings.value("daemon_port", 18081).toInt();

    wallet_uri = settings.value("wallet_uri", "localhost").toString();
    wallet_port = settings.value("wallet_port", 19091).toInt();

    miner_uri = settings.value("miner_uri", "localhost").toString();
    miner_port = settings.value("miner_port", 18081).toInt();

    wallet_program = settings.value("wallet_program","/usr/bin/simplewallet").toString();

    mining_enabled = settings.value("mining_enabled",false).toBool();;
    mining_address = settings.value("miner_mining_address","").toString();


    wallet_ip = settings.value("wallet_bind_ip", "127.0.0.1").toString();
    wallet_file = settings.value("wallet_file", QDir::homePath() + "/.bitmonero/wallet").toString();
    wallet_password = settings.value("wallet_password","").toString();


    spawn_wallet = settings.value("spawn_wallet", true).toBool() ;
}


bool WalletSettings::saveWalletConfiguration()
{

    if ( !areSettingsAcceptable() ) {
        return false;
    }

    settings.setValue("wallet_program", wallet_program);
    settings.setValue("wallet_file", wallet_file);
    settings.setValue("wallet_password", wallet_password);

    return true;

}
