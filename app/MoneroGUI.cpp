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


MoneroGUI::MoneroGUI(QGuiApplication& pApp, const QString& pCustomConfigFile)
    : app(pApp), monero_interface(NULL), miner_interface(NULL), wallet_interface(NULL), wallet_handler_interface(NULL), settings(pCustomConfigFile), exit_status(0)
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

    engine.rootContext()->setContextProperty("application", &application_model);
    engine.rootContext()->setContextProperty("monero", &monero_model);
    engine.rootContext()->setContextProperty("wallet", &wallet_model);
    engine.rootContext()->setContextProperty("miner", &miner_model);
    engine.rootContext()->setContextProperty("wallet_handler", &wallet_handler_model);


    engine.rootContext()->setContextProperty("settings", &settings);

}



void MoneroGUI::createMonero() {

    if (monero_interface) {
        delete monero_interface;
        monero_interface = NULL;
    }

    monero_interface = new RPCMonero(monero_model,settings);
}

void MoneroGUI::createMiner() {

    if (miner_interface) {
        delete miner_interface;
        miner_interface = NULL;
    }

    MinerInterfaceBuilder lMinerInterfaceBuilder(miner_model, settings);
    miner_interface = lMinerInterfaceBuilder.buildInterface();
}

void MoneroGUI::createWalletHandler() {

    if (wallet_handler_interface) {
        delete wallet_handler_interface;
        wallet_handler_interface = NULL;
    }

    WalletHandlerInterfaceBuilder lWalletHandlerInterfaceBuilder(wallet_handler_model, settings);
    wallet_handler_interface = lWalletHandlerInterfaceBuilder.buildInterface();
}

void MoneroGUI::createWallet() {

    if (wallet_interface) {
        delete wallet_interface;
        wallet_interface = NULL;
    }

    WalletInterfaceBuilder lWalletInterfaceBuilder(wallet_model,settings);
    wallet_interface = lWalletInterfaceBuilder.buildInterface();
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

    qDebug() << "Launching main window...";

    closeAllWindows();

    /* Starts the main app */
    if (!initMainWindow(engine)) {
        qCritical() << "MainWindow init failed. Aborting...";
        return -1;
    }

    QObject::connect(&application_model, SIGNAL(onApplicationQuitRequested()), &app, SLOT(quit()), Qt::UniqueConnection);
    QObject::connect(&application_model, SIGNAL(onLaunchWizardRequested()), this, SLOT(reconfigure()), Qt::UniqueConnection);


    return 0;
}

void MoneroGUI::reconfigure()
{
    reconfiguration_requested = true;
    stepConfigure();
    reconfiguration_requested = false;
}

void MoneroGUI::closeAllWindows() {

    for ( QWindow* lWindow : app.allWindows() ) {
        lWindow->close();
    }
}




void MoneroGUI::stepEnableDaemon() {
    qDebug() << "[STEP] Enable Daemon";

    createMonero();

    QObject::connect(monero_interface,SIGNAL(ready()),this, SLOT(stepConfigure()), Qt::UniqueConnection);
    QObject::connect(monero_interface,SIGNAL(fatalError(int)), this, SLOT(daemonError(int)), Qt::UniqueConnection);

    monero_interface->enable();

}

void MoneroGUI::stepConfigure() {

    qDebug() << "[STEP] Configure";
    createWalletHandler();
    createWallet();
    createMiner();

    if ( exit_status != 0 ) {
        return;
    }
    qWarning() << "[OK] Monero";


    if( !isReady() )  {
        qWarning() << "Not configured. Starting wizard";

        /* Allow to pass to next step */
        /* Avoids duplicates */
        QObject::connect(&application_model, SIGNAL(onWizardSuccess()), this, SLOT(stepEnableWalletHandler()), Qt::UniqueConnection);
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

    QObject::connect(wallet_handler_interface, SIGNAL(ready()), this, SLOT(stepEnableWallet()), Qt::UniqueConnection);
    QObject::connect(wallet_handler_interface, SIGNAL(fatalError(int)), this, SLOT(walletHandlerError(int)), Qt::UniqueConnection);
    wallet_handler_interface->enable();

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
        dialogError(5);
        return;
    }

    QObject::connect(wallet_interface, SIGNAL(ready()), this, SLOT(stepStartMainGUI()), Qt::UniqueConnection);
    QObject::connect(wallet_interface, SIGNAL(fatalError(int)), this, SLOT(walletError(int)), Qt::UniqueConnection);
    wallet_interface->enable();

}

void MoneroGUI::stepStartMainGUI() {

    qWarning() << "[OK] Wallet";

    /* At this stage, we can consider that the program configuration is sane. Let's save it */
    settings.saveWalletConfiguration();

    /* Doesn't wait for minre to be ready */
    QObject::connect(miner_interface, SIGNAL(fatalError(int)), this, SLOT(minerError(int)), Qt::UniqueConnection);

    startMainWindow();
    /* Will block until main window is closed */

}


int MoneroGUI::start() {

    std::cout << settings << std::endl;

    initModels();

    startup_timer.setSingleShot(true);
    connect(&startup_timer, SIGNAL(timeout()), this, SLOT(stepEnableDaemon()));
    startup_timer.start(2000);
    qWarning() << "Start SPLASH";
    startSplashScreen();


    qDebug() << "Closing remaining Windows...";
    closeAllWindows();


    qDebug() << "exit_status : " << exit_status;

    /* Unhandler errors */
    if (exit_status != 0) {

        dialogError(exit_status);

    }
    else {

//        initHappyEnding(engine);
//        app.processEvents();

    }

    return exit_status;

}


void MoneroGUI::daemonError(int pCode)
{
    qCritical() << "Daemon error : " << pCode;
    dialogError(10+pCode);
}

void MoneroGUI::walletError(int pCode)
{
    qCritical() << "Wallet error : " << pCode;
    dialogError(20+pCode);
}

void MoneroGUI::walletHandlerError(int pCode)
{
    qCritical() << "WalletHandler error : " << pCode;
    dialogError(30+pCode);
}

void MoneroGUI::minerError(int pCode)
{
    qCritical() << "Miner error : " << pCode;
    dialogError(40+pCode);
}

void MoneroGUI::dialogError(int pErrorCode) {

    closeAllWindows();

    qDebug() << "Opening error Window";
    engine.rootContext()->setContextProperty("error_message", "Error !!");
    engine.rootContext()->setContextProperty("error_code", pErrorCode);
    initQmlElement(engine, QUrl("qrc:/qml/ErrorWindow.qml"));

    QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));

    app.exec();

}
