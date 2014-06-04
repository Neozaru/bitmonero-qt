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
#include "DaemonHandler.h"

#include "Utils.h"

bool createComponent(QQmlComponent& pComponent) {

    pComponent.create();
    if ( !pComponent.isReady() ) {
        qDebug() << "Component not ready";
        qDebug() << "Error " << pComponent.errors();
        return false;
    }

    return true;

}


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);


    /* Loads settings. TODO : Put an interface in the GUI */
    WalletSettings lWalletSettings;


    std::cout << lWalletSettings << std::endl;


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


    /* Pushing models into views */
    QQmlEngine lEngine;


    /* RAII */
    MoneroInterface* lMonero = new RPCMonero(lMoneroModel,lWalletSettings);
    WalletInterface* lWallet = new RPCWallet(lWalletModel, lWalletSettings);
    MinerInterface* lMiner = new RPCMiner(lMinerModel, lWalletSettings.getMinerUri(), lWalletSettings.getMinerPort());

    /* TODO : Use builder and/or abstracted interface */
    WalletHandler lWalletHandler(lWalletSettings);

    /* Check if bitmonerod daemon is ready */
    if (!lMonero->isReady()) {
        qCritical() << "Monero daemon error : Not started. Aborting.";
        return 3;
    }

    if (!lWalletHandler.isOk()) {
        qCritical() << "Wallet program error : Not executable. Abording.";
    }

    lEngine.rootContext()->setContextProperty("monero", &lMoneroModel);
    lEngine.rootContext()->setContextProperty("wallet", &lWalletModel);
    lEngine.rootContext()->setContextProperty("miner", &lMinerModel);


    lEngine.rootContext()->setContextProperty("wallet_handler", &lWalletHandler);
    lEngine.rootContext()->setContextProperty("settings", &lWalletSettings);


    /* Allow to exit the application */
    QObject::connect(&lEngine,SIGNAL(quit()),&app,SLOT(quit()));

    /* Wizard instructions */
    {
        bool lRunMainWindow = lWalletSettings.areSettingsAcceptable();
        if (!lRunMainWindow) {
            QQmlComponent lComponent(&lEngine, QUrl("qrc:/qml/wizard.qml"));
            if ( !createComponent(lComponent) ) {
                qDebug() << "Aborting";
                return 1;
            }

            /* Starts the wizard */
            app.exec();
        }
    }

    std::cout << "[New config]" << std::endl;
    std::cout << lWalletSettings << std::endl;

    /* Main Window instructions */
    {
        bool lRunMainWindow = lWalletSettings.areSettingsAcceptable();

        if (lRunMainWindow) {

            /* spawn_wallet defaults to true when password is set in config. Check if user disabled wallet opening */
            if ( lWalletSettings.shouldSpawnWallet() ) {

                qDebug() << "Checking Wallet...";
                if ( !lWalletHandler.tryWallet(lWalletSettings.getWalletFile(), lWalletSettings.getWalletPassword()) ) {
                    qWarning() << "Wallet opening failed. Aborting.";
                    return 2;
                }

                qDebug() << "[OK]";
                qDebug() << "Opening Wallet...";
                if ( !lWalletHandler.openWalletAsync(lWalletSettings.getWalletFile(), lWalletSettings.getWalletPassword(), lWalletSettings.getWalletIP(), lWalletSettings.getWalletPort()) ) {
                    qDebug() << "Failed to start wallet ("<< lWalletSettings.getWalletProgram() << ")";
                    return 2;
                }

            }
            else {
                qDebug() << "'spawn_wallet' disabled. Connecting to existing wallet on port" << lWalletSettings.getWalletPort();
            }



            /* At this stage, we can consider that the program configuration is sane. Let's save it */
            lWalletSettings.saveWalletConfiguration();

            QQmlComponent lComponent(&lEngine, QUrl("qrc:/qml/main.qml"));


            if ( !createComponent(lComponent) ) {
                qDebug() << "Aborting";
                return 1;
            }

            /* Starts the main app */
            int lReturnCode = app.exec();
            qDebug() << "End of processes";

            lWalletHandler.closeWallet();
//            lDaemonHandler.kill();
            delete lMonero;

            return lReturnCode;
        }

    }

//    lDaemonHandler.kill();

    qDebug() << "Not configured";
    return 0;

}
