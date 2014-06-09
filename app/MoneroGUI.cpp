#include "MoneroGUI.h"


#include "RPC/RPCMonero.h"
#include "RPC/RPCWallet.h"
#include "RPC/RPCMiner.h"

#include <QQuickWindow>


MoneroGUI::MoneroGUI(QGuiApplication& pApp)
    : app(pApp), monero_interface(NULL), miner_interface(NULL), wallet_interface(NULL), wallet_handler(settings), exit_status(0), splashscreen_running(false)
{

}

MoneroGUI::~MoneroGUI() {

    if (wallet_interface) {
        delete wallet_interface;
    }

    if (monero_interface) {
        delete monero_interface;
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


    engine.rootContext()->setContextProperty("settings", &settings);

}

void MoneroGUI::initInterfaces() {

    monero_interface = new RPCMonero(monero_model,settings);
    wallet_interface = new RPCWallet(wallet_model, settings);
    miner_interface = new RPCMiner(miner_model, settings.getMinerUri(), settings.getMinerPort());

}

int MoneroGUI::startWizard()
{

    stopSplashScreen();

    initWizard(engine);
    /* Starts the wizard */
    return app.exec();

}

int MoneroGUI::startMainWindow()
{

    stopSplashScreen();

    if (!initMainWindow(engine)) {
        qCritical() << "MainWindow init failed. Aborting...";
        return -1;
    }

    QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));
    QObject::connect(&engine, SIGNAL(destroyed()), &app, SLOT(quit()));

    /* Starts the main app */
    return app.exec();
}

void MoneroGUI::onMainWindowQuit(int pReturnCode) {
    qWarning() << ":::onMainWindowQuit : " << pReturnCode;
//    emit this->applicationQuit();
}

void MoneroGUI::stopSplashScreen() {

    if ( splashscreen_running && !app.allWindows().empty() ) {
        app.allWindows().first()->close();
        splashscreen_running = false;
    }

}

int MoneroGUI::start() {

    std::cout << settings << std::endl;

    initModels();
    initInterfaces();


    QObject::connect(monero_interface, &MoneroInterface::ready, [=]() {
        qWarning() << "MONERO READY";

        /* Unlocks main loop */
        app.quit();
    });

    int lDaemonReturnCode = monero_interface->enable();
    if ( lDaemonReturnCode != 0 ) {

        /* Starnge error codes :D */
        dialogError(10 + lDaemonReturnCode);
        return 10 + lDaemonReturnCode;
    }

    qWarning() << "Start SPLASH";

    /* Blocking */
    startSplashScreen();


    qWarning() << "[OK] Monero";

    /* TODO : Use builder and/or abstracted interface */
    engine.rootContext()->setContextProperty("wallet_handler", &wallet_handler);


    /* Yet another hack. To refactor */
    if (!wallet_handler.isOk()) {
        qCritical() << "Wallet program error : Not executable. Abording.";
        dialogError(21);
        return 21;
    }

    /* Allow to exit the application */
    QObject::connect(&engine,SIGNAL(quit()),&app,SLOT(quit()));

    if( !isReady() )  {
        qWarning() << "Not configured. Starting wizard";
        startWizard();
        /* Blocks */
        qWarning() << "WIzard exited ";
    }


    std::cout << "[New config]" << std::endl;
    std::cout << settings << std::endl;

    /* Main Window instructions */
    if (isReady()) {

        /* Will start the application when wallet ready */
        QObject::connect(wallet_interface, &WalletInterface::ready, [this]() {

            /* At this stage, we can consider that the program configuration is sane. Let's save it */
            settings.saveWalletConfiguration();

            qWarning() << "[OK] Wallet";

            int lReturnCode = startMainWindow();
            /* Will block until main window is closed */

            emit onMainWindowQuit(lReturnCode);

        });
        int lWalletReturnCode = wallet_interface->enable();

        if ( lWalletReturnCode != 0 ) {
            dialogError(20+lWalletReturnCode);
            return 20+lWalletReturnCode;
        }


        QEventLoop lBlockingLoop;
        QObject::connect(this, SIGNAL(applicationQuit(int)), &lBlockingLoop, SLOT(quit()));
        lBlockingLoop.exec();
        /* Will block until onMainWindowQuit is triggered */

        wallet_handler.closeWallet();

    }
    else {
        exit_status = 5;
    }

    qDebug() << "exit_status : " << exit_status;

    if (exit_status != 0) {

        dialogError(exit_status);

    }


    return exit_status;

}

void MoneroGUI::dialogError(int pErrorCode) {

    if ( !app.allWindows().empty() ) {
        app.allWindows().first()->close();
    }

    qDebug() << "Opening error Window";
    engine.rootContext()->setContextProperty("error_message", "Error !!");
    engine.rootContext()->setContextProperty("error_code", pErrorCode);
    initQmlElement(engine, QUrl("qrc:/qml/ErrorWindow.qml"));

    QObject::connect(&engine, SIGNAL(quit()), &app, SLOT(quit()));

    app.exec();

}
