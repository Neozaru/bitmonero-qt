#include <QObject>

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
#include <QFile>

#include "Models/MoneroModel.h"
#include "Models/WalletModel.h"
#include "Models/MinerModel.h"


#include "RPC/RPCMonero.h"
#include "RPC/RPCWallet.h"
#include "RPC/RPCMiner.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    /* Loads settings. TODO : Put an interface in the GUI */
    const QString& lConfigFile = QDir::homePath() + "/.bitmonero-qt/bitmonero-qt.conf";
    QSettings lSettings(lConfigFile, QSettings::IniFormat);


    /* Used for auto creating config file */
    if ( !lSettings.value("mining_enabled").isValid() ) {
        lSettings.setValue("mining_enabled", false);
    }

    bool lMiningEnabled = lSettings.value("mining_enabled",false).toBool();

    const QString& lMoneroUri = lSettings.value("daemon_uri", "localhost/json_rpc").toString();
    int lMoneroPort = lSettings.value("daemon_port", 18081).toInt();

    const QString& lWalletUri = lSettings.value("wallet_uri", "localhost").toString();
    int lWalletPort = lSettings.value("wallet_port", 19091).toInt();

    const QString& lMinerUri = lSettings.value("miner_uri", "localhost").toString();
    int lMinerPort = lSettings.value("miner_port", 18081).toInt();
    const QString& lMinerAddress = lSettings.value("miner_mining_address","").toString();


    qDebug() << "[Loaded config]";
    qDebug() << "Daemon : " << lMoneroUri << " " << lMoneroPort;
    qDebug() << "Wallet : " << lWalletUri << " " << lWalletPort;
    qDebug() << "Miner : " << lMinerUri << " " << lMinerPort;
    /* */


    qmlRegisterType<WalletModel>("info.neozaru.bitmonero-qt.walletmodel", 1, 0, "WalletModel");

    MoneroModel lMoneroModel;
    WalletModel lWalletModel;
    MinerModel lMinerModel;


    /* If a custom mining address is configured, assign it. Use wallet address otherwise */
    if ( !lMinerAddress.isEmpty() ) {
        lMinerModel.setAddress(lMinerAddress);
    }
    else {
        QObject::connect(&lWalletModel,SIGNAL(addressChanged(QString)),&lMinerModel,SLOT(setAddress(QString)));
    }


    /* Set up RPC interfaces. TODO : Create a builder */
    //    MoneroInterface* monero = new RPCMonero(lMoneroUri, lMoneroPort);

    /* RAII */
    WalletInterface* lWallet = new RPCWallet(lWalletModel, lWalletUri, lWalletPort);
    MinerInterface* lMiner = new RPCMiner(lMinerModel, lMinerUri, lMinerPort);


    /* Pushing models into views */
    QQmlEngine engine;
    engine.rootContext()->setContextProperty("monero", &lMoneroModel);
    engine.rootContext()->setContextProperty("wallet", &lWalletModel);
    engine.rootContext()->setContextProperty("miner", &lMinerModel);

    QQmlComponent component(&engine, QUrl("qrc:/qml/main.qml"));

    component.create();
    if ( !component.isReady() ) {
        qDebug() << "Component not ready";
        qDebug() << "Error " << component.errors();
        return 2;
    }

    /* Allow to exit the application */
    QObject::connect(&engine,SIGNAL(quit()),&app,SLOT(quit()));

    return app.exec();
}
