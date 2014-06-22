#include "MoneroGUI.h"

#include "Builders/WalletInterfaceBuilder.h"
#include "Builders/WalletHandlerInterfaceBuilder.h"
#include "Builders/MinerInterfaceBuilder.h"

#include "Interfaces/WalletInterface.h"
#include "Interfaces/WalletHandlerInterface.h"
#include "Interfaces/MinerInterface.h"

#include "RPC/RPCMonero.h"
//#include "RPC/RPCWallet.h"
//#include "RPC/RPCMiner.h"
//#include "RPC/WalletHandlerProcess.h"

#include <QQuickWindow>


MoneroGUI::MoneroGUI(QGuiApplication& pApp)
    : app(pApp), monero_interface(NULL), miner_interface(NULL), wallet_interface(NULL), wallet_handler_interface(NULL), exit_status(0)
{

}

MoneroGUI::~MoneroGUI() {

    try {
        if (miner_interface) {
            qWarning() << "Deleting miner interface ...";
            delete miner_interface;
        }


        if (wallet_interface) {
            qWarning() << "Deleting wallet interface ...";
            delete wallet_interface;
        }

        if (wallet_handler_interface) {
            qWarning() << "Deleting wallet_handler interface ...";
            delete wallet_handler_interface;
        }

        if (monero_interface) {
            qWarning() << "Deleting monero interface ...";
            delete monero_interface;
        }



    }
    catch (const std::exception& ex) {
        qDebug() << "Catched std::exception : " << ex.what();
    }
    catch (const std::string& ex) {
        qDebug() << "Catched std::string : " << QString::fromStdString(ex);
    }
    catch (...) {
        qDebug() << "Catched unknown exception";
    }
}

void MoneroGUI::initModels() {


    /* If a custom mining address is configured, assign it. Use wallet address otherwise */
    if ( settings.isMiningAddressConfigured() ) {
        miner_model.setAddress(settings.getMiningAddress());
    }
    else {
        QObject::connect(&wallet_model,SIGNAL(addressChanged(QString)),&miner_model,SLOT(setAddress(QString)));
    }


    engine.rootContext()->setContextProperty("monero", &monero_model);
    engine.rootContext()->setContextProperty("wallet", &wallet_model);
    engine.rootContext()->setContextProperty("miner", &miner_model);
    engine.rootContext()->setContextProperty("wallet_handler", &wallet_handler_model);


    engine.rootContext()->setContextProperty("settings", &settings);

}

void MoneroGUI::initInterfaces() {

    monero_interface = new RPCMonero(monero_model,settings);

    WalletInterfaceBuilder lWalletInterfaceBuilder(wallet_model,settings);
    wallet_interface = lWalletInterfaceBuilder.buildInterface();
//    wallet_interface = new RPCWallet(wallet_model, settings);

//    miner_interface = new RPCMiner(miner_model, settings.getMinerUri(), settings.getMinerPort());
    MinerInterfaceBuilder lMinerInterfaceBuilder(miner_model, settings);
    miner_interface = lMinerInterfaceBuilder.buildInterface();

    WalletHandlerInterfaceBuilder lWalletHandlerInterfaceBuilder(wallet_handler_model, settings);
    wallet_handler_interface = lWalletHandlerInterfaceBuilder.buildInterface();

//    wallet_handler_interface = new WalletHandlerProcess(wallet_handler_model, settings);

}

int MoneroGUI::startWizard()
{

    closeAllWindows();

    /* Starts the wizard */
    if ( !initWizard(engine) ) {
        qCritical() << "Wizard init failed. Aborting...";
        return -1;
    }


    return 0;

}

int MoneroGUI::startMainWindow()
{

    closeAllWindows();

    /* Starts the main app */
    if (!initMainWindow(engine)) {
        qCritical() << "MainWindow init failed. Aborting...";
        return -1;
    }

    QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));
    QObject::connect(&engine, SIGNAL(destroyed()), &app, SLOT(quit()));


    return 0;
}


void MoneroGUI::closeAllWindows() {

    for ( QWindow* lWindow : app.allWindows() ) {
        lWindow->close();
    }
}




bool MoneroGUI::stepEnableDaemon() {
    qDebug() << "[STEP] Enable Daemon";

    QObject::connect(monero_interface,SIGNAL(ready()),this, SLOT(stepConfigure()));
    int lDaemonReturnCode = monero_interface->enable();
    if ( lDaemonReturnCode != 0 ) {

        QObject::disconnect(monero_interface,SIGNAL(ready()),this, SLOT(stepConfigure()));
        /* Starnge error codes :D */
        exit_status = 10 + lDaemonReturnCode;

        return false;
    }

    return true;

}

void MoneroGUI::stepConfigure() {
    qDebug() << "[STEP] Configure";

    if ( exit_status != 0 ) {
        return;
    }
    qWarning() << "[OK] Monero";


    if( !isReady() )  {
        qWarning() << "Not configured. Starting wizard";

        /* Allow to pass to next step */
        QObject::connect(&engine,SIGNAL(quit()),this,SLOT(stepEnableWalletHandler()));
        startWizard();
        /* Blocks */
        qWarning() << "Wizard exited ";
    }
    else {
        stepEnableWalletHandler();
    }



}

void MoneroGUI::stepEnableWalletHandler() {
    qDebug() << "[STEP] Enable WalletHandler";

    QObject::connect(wallet_handler_interface, SIGNAL(ready()), this, SLOT(stepEnableWallet()));

    int lReturnCode = wallet_handler_interface->enable();
    if ( lReturnCode != 0 ) {
        exit_status = 30 + lReturnCode;
        dialogError(exit_status);
        return;
    }


}


void MoneroGUI::stepEnableWallet() {
    qDebug() << "[STEP] Enable Wallet";

    if ( exit_status != 0 ) {
        return;
    }

    std::cout << "[New config]" << std::endl;
    std::cout << settings << std::endl;


    /* Main Window instructions */
    if (!isReady()) {
        exit_status = 5;
        return;
    }

    QObject::connect(wallet_interface, SIGNAL(ready()), this, SLOT(stepStartMainGUI()));

    int lWalletReturnCode = wallet_interface->enable();

    if ( lWalletReturnCode != 0 ) {
        exit_status = 20+lWalletReturnCode;
        dialogError(exit_status);
    }


}

void MoneroGUI::stepStartMainGUI() {

    qWarning() << "[OK] Wallet";

    /* At this stage, we can consider that the program configuration is sane. Let's save it */
    settings.saveWalletConfiguration();

    if ( miner_interface->enable() != 0 ) {
        qWarning() << "Error occured while initializing 'miner_interface'";
    }

    startMainWindow();
    /* Will block until main window is closed */

}


int MoneroGUI::start() {

    std::cout << settings << std::endl;

    initModels();
    initInterfaces();

    if( stepEnableDaemon() ) {

        qWarning() << "Start SPLASH";

        /* Blocking */
        startSplashScreen();

    }

    qDebug() << "Closing remaining Windows...";
    closeAllWindows();


    qDebug() << "exit_status : " << exit_status;

    if (exit_status != 0) {

        dialogError(exit_status);

    }

    return exit_status;

}


void MoneroGUI::dialogError(int pErrorCode) {

    closeAllWindows();
    app.quit();

    qDebug() << "Opening error Window";
    engine.rootContext()->setContextProperty("error_message", "Error !!");
    engine.rootContext()->setContextProperty("error_code", pErrorCode);
    initQmlElement(engine, QUrl("qrc:/qml/ErrorWindow.qml"));

    QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));

    app.exec();

}
