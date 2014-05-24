#include <QCoreApplication>

#include <QGuiApplication>
#include <QQuickWindow>
#include <QDebug>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

#include "Models/MoneroModel.h"
#include "Models/WalletModel.h"
#include "Models/MinerModel.h"


#include "RPC/RPCMonero.h"
#include "RPC/RPCWallet.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    /* Loads settings. TODO : Put an interface in the GUI */
    const QString& lConfigFile = QDir::homePath() + "/.bitmonero-qt/bitmonero-qt.conf";
    QSettings lSettings(lConfigFile, QSettings::IniFormat);

    lSettings.beginGroup("MainConfig");

    const QString& lMoneroUri = lSettings.value("daemon_uri", "localhost/json_rpc").toString();
    int lMoneroPort = lSettings.value("daemon_port", 18081).toInt();

    const QString& lWalletUri = lSettings.value("wallet_uri", "localhost").toString();
    int lWalletPort = lSettings.value("wallet_port", 19091).toInt();

    const QString& lMinerUri = lSettings.value("miner_uri", "localhost").toString();
    int lMinerPort = lSettings.value("miner_port", 19091).toInt();

    lSettings.endGroup();


    qDebug() << "[Loaded config]";
    qDebug() << "Daemon : " << lMoneroUri << " " << lMoneroPort;
    qDebug() << "Wallet : " << lWalletUri << " " << lWalletPort;
    qDebug() << "Miner : " << lMinerUri << " " << lMinerPort;
    /* */


    qmlRegisterType<WalletModel>("info.neozaru.bitmonero-qt.walletmodel", 1, 0, "WalletModel");

    MoneroInterface* monero = new RPCMonero("localhost/json_rpc",18081);

    MoneroModel lMoneroModel;

    WalletModel lWalletModel;
    WalletInterface* lWallet = new RPCWallet(lWalletModel,"localhost",19091);

    MinerModel lMinerModel;


    QQmlEngine engine;
    engine.rootContext()->setContextProperty("monero", &lMoneroModel);
    engine.rootContext()->setContextProperty("wallet", &lWalletModel);
    engine.rootContext()->setContextProperty("miner", &lMinerModel);

    QQmlComponent component(&engine, QUrl("qrc:/qml/main.qml"));

    component.create();
    if ( !component.isReady() ) {
        qDebug() << "/!\ Component not ready";
        qDebug() << "Error " << component.errors();
        return 2;
    }

    return app.exec();
}
