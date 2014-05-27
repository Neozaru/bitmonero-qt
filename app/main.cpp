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

#include <QProcess>
#include <QThread>
#include <QStringList>

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


    QProcess* lWalletProcess = new QProcess();
    /* If a password was set, starts the Wallet as subprocess */
    if ( lSettings.contains("wallet_password") ) {

        if ( lSettings.value("spawn_wallet", true).toBool() ) {

            const QString& lWalletFile = lSettings.value("wallet_file", QDir::homePath() + "/.bitmonero/wallet.bin").toString();
            const QString& lWalletPassword = lSettings.value("wallet_password").toString();
            const QString& lWalletProgram = lSettings.value("wallet_program","/usr/bin/simplewallet").toString();
            const QString& lWalletIP = lSettings.value("wallet_bind_ip", "127.0.0.1").toString();


            QStringList lArguments;

            lArguments.append("--wallet=" + lWalletFile);
            lArguments.append("--pass=" + lWalletPassword);
            lArguments.append("--rpc-bind-port=" + QString::number(lWalletPort));
            lArguments.append("--rpc-bind-ip=" + lWalletIP);

            lWalletProcess->start(lWalletProgram, lArguments);

            qDebug() << "Wallet process started on " + lWalletIP + ":" + QString::number(lWalletPort) + " (" + lWalletProgram + ")";

            QThread::sleep(3);

        }
        else {
            qDebug() << "'spawn_wallet' disabled. Connecting to existing wallet on port" << lWalletPort;
        }


    }
    else {
        qDebug() << "Sèche linge";
        qDebug() << "Wallet configuration not found : 'wallet_password'. Please ensure an RPC wallet (simplewallet) is running on port" << lWalletPort;
    }


    /* So we can finally start (TODO: Refactor init/config process) */


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


    app.processEvents();
    /* Allow to exit the application */
    QObject::connect(&engine,SIGNAL(quit()),&app,SLOT(quit()));

    int lReturnCode = app.exec();


    qDebug() << "End of processes";


    lWalletProcess->kill();

    return lReturnCode;
}
