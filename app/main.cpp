#include <iostream>

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

#include "WalletSettings.h"
#include "WalletHandler.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    /* Loads settings. TODO : Put an interface in the GUI */
    WalletSettings lWalletSettings;


    std::cout << lWalletSettings;
    WalletHandler lWalletHandler(lWalletSettings.getWalletProgram());

    /* If a password was set, starts the Wallet as subprocess */
    if ( lWalletSettings.isWalletPasswordDefined() ) {

        /* spawn_wallet defaults to true when password is set in config. Check if user disabled wallet opening */
        if ( lWalletSettings.shouldSpawnWallet() ) {

            if ( !lWalletHandler.openWalletAsync(lWalletSettings.getWalletFile(), lWalletSettings.getWalletPassword(), lWalletSettings.getWalletIP(), lWalletSettings.getWalletPort()) ) {
                qDebug() << "Failed to start wallet ("<< lWalletSettings.getWalletProgram() << ")";
            }

        }
        else {
            qDebug() << "'spawn_wallet' disabled. Connecting to existing wallet on port" << lWalletSettings.getWalletPort();
        }


    }
    else {
        qDebug() << "Sèche linge";
        qDebug() << "Wallet configuration not found : 'wallet_password'. Please ensure an RPC wallet (simplewallet) is running on port" << lWalletSettings.getWalletPort();
    }


    /* So we can finally start (TODO: Refactor init/config process) */


    MoneroModel lMoneroModel;
    WalletModel lWalletModel;
    MinerModel lMinerModel;


    /* If a custom mining address is configured, assign it. Use wallet address otherwise */
    if ( lWalletSettings.isMiningAddressConfigured() ) {
        lMinerModel.setAddress(lWalletSettings.getMiningAddress());
    }
    else {
        QObject::connect(&lWalletModel,SIGNAL(addressChanged(QString)),&lMinerModel,SLOT(setAddress(QString)));
    }


    /* Set up RPC interfaces. TODO : Create a builder */
    //    MoneroInterface* monero = new RPCMonero(lMoneroUri, lMoneroPort);

    /* RAII */
    WalletInterface* lWallet = new RPCWallet(lWalletModel, lWalletSettings.getWalletUri(), lWalletSettings.getWalletPort());
    MinerInterface* lMiner = new RPCMiner(lMinerModel, lWalletSettings.getMinerUri(), lWalletSettings.getMinerPort());


    /* Pushing models into views */
    QQmlEngine engine;
    engine.rootContext()->setContextProperty("monero", &lMoneroModel);
    engine.rootContext()->setContextProperty("wallet", &lWalletModel);
    engine.rootContext()->setContextProperty("miner", &lMinerModel);

    engine.rootContext()->setContextProperty("wallet_handler", &lWalletHandler);

    QQmlComponent component(&engine, QUrl("qrc:/qml/main.qml"));
//    QQmlComponent component(&engine, QUrl("qrc:/qml/wizard.qml"));

    component.create();
    if ( !component.isReady() ) {
        qDebug() << "Component not ready";
        qDebug() << "Error " << component.errors();
        return 2;
    }


//    app.processEvents();
    /* Allow to exit the application */
    QObject::connect(&engine,SIGNAL(quit()),&app,SLOT(quit()));


    qDebug() << "STARTING APP";
    int lReturnCode = app.exec();


    qDebug() << "End of processes";


    lWalletHandler.closeWallet();

    return lReturnCode;
}
