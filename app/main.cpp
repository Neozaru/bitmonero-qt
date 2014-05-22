#include <QCoreApplication>

#include <QGuiApplication>
#include <QQuickWindow>
#include <QDebug>

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "Models/WalletModel.h"
#include "RPC/RPCMonero.h"
#include "RPC/RPCWallet.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MoneroInterface* monero = new RPCMonero("localhost/json_rpc",18081);

    WalletModel wm;
    WalletInterface* w = new RPCWallet(wm,"localhost",19091);

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("wallet", &wm);

    QQmlComponent component(&engine, QUrl("qrc:/qml/main.qml"));

    component.create();
    if ( !component.isReady() ) {
        qDebug() << "/!\ Component not ready";
        qDebug() << "Error " << component.errors();
    }

    return app.exec();
}
