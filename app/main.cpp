#include <QCoreApplication>

#include <QGuiApplication>
#include <QQuickWindow>
#include <QDebug>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "Models/MoneroModel.h"
#include "Models/WalletModel.h"
#include "Models/MinerModel.h"


#include "RPC/RPCMonero.h"
#include "RPC/RPCWallet.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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
