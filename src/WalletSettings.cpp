#include "WalletSettings.h"



WalletSettings::WalletSettings(const QString& pConfigFile)
    : settings(pConfigFile, QSettings::IniFormat)
{
    /* Used for auto creating config file */
    if ( !settings.value("mining_enabled").isValid() ) {
        settings.setValue("mining_enabled", false);
    }


    monero_uri = settings.value("daemon_uri", "localhost").toString();
    monero_port = settings.value("daemon_port", 18081).toInt();

    wallet_uri = settings.value("wallet_uri", "localhost").toString();
    wallet_port = settings.value("wallet_port", 19091).toInt();

    miner_uri = settings.value("miner_uri", "localhost").toString();
    miner_port = settings.value("miner_port", 18081).toInt();

    wallet_cli_program = settings.value("wallet_cli_program", "").toString();
    wallet_rpc_program = settings.value("wallet_rpc_program", "").toString();
    daemon_program = settings.value("daemon_program", "").toString();

    mining_enabled = settings.value("mining_enabled",false).toBool();;
    mining_address = settings.value("miner_mining_address","").toString();


    wallet_ip = settings.value("wallet_bind_ip", "127.0.0.1").toString();
    wallet_file = settings.value("wallet_file", QDir::homePath() + "/.bitmonero/wallet").toString();
    wallet_password = settings.value("wallet_password","").toString();


    spawn_wallet = settings.value("spawn_wallet", true).toBool();
    spawn_daemon = settings.value("spawn_daemon", true).toBool();
    daemon_kill_delay = settings.value("daemon_kill_delay", 15).toInt();

    wallet_interface = settings.value("wallet_interface", "RPC").toString();
    wallet_handler_interface = settings.value("wallet_handler_interface", "RPC").toString();
    miner_interface = settings.value("miner_interface", "RPC").toString();


}


bool WalletSettings::saveWalletConfiguration()
{

    if ( !areWalletSettingsAcceptable() ) {
        return false;
    }

    /* Option which are settable with the GUI */
    settings.setValue("wallet_cli_program", wallet_cli_program);
    settings.setValue("wallet_rpc_program", wallet_rpc_program);
    settings.setValue("wallet_file", wallet_file);
    settings.setValue("wallet_password", wallet_password);

    return true;

}

bool WalletSettings::saveGlobalConfiguration() {

//    TODO : Check
    settings.setValue("daemon_uri", monero_uri);
    settings.setValue("daemon_port", monero_port);

    return true;
}
