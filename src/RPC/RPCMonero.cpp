#include "RPCMonero.h"

#include <QString>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QEventLoop>

#include <QJsonObject>

#include "JsonRPCRequest.h"

RPCMonero::RPCMonero(MoneroModel& pMoneroModel, const WalletSettings& pSettings)
    : MoneroInterface(pMoneroModel), daemon_handler(pSettings), rpc(pSettings.getMoneroUri(),pSettings.getMoneroPort())
{
    should_spawn_daemon = pSettings.shouldSpawnDaemon();
}

RPCMonero::~RPCMonero() {

    getinfo_timer.stop();
    savebc_timer.stop();

}

void RPCMonero::getInfo()
{

    JsonRPCRequest* lReq = rpc.sendRequest("getinfo",QJsonObject(), true);
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {

        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {

            this->onReady();


            this->onInfoUpdated(
                pJsonResponse["height"].toInt(),
                pJsonResponse["target_height"].toInt(),
                pJsonResponse["difficulty"].toInt(),
                pJsonResponse["incoming_connections_count"].toInt(),
                pJsonResponse["outgoing_connections_count"].toInt()
            );

        }
        else {
            qWarning() << "Bad status for 'getinfo' start : " << lStatus;

        }
    });

}

void RPCMonero::saveBlockchain()
{

    JsonRPCRequest* lReq = rpc.sendRequest("save_bc",QJsonObject(), true);
    QObject::connect(lReq,&JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {
        //NOP
    });

}



void RPCMonero::enable()
{



    if (should_spawn_daemon) {

        if (!daemon_handler.isOk()) {
            qCritical() << "Configured to spawn 'bitmonerod' daemon but no executable found. Aborting...";
            this->onFatalError(1);
            return;
        }

        if (!daemon_handler.execDaemon()) {
            qCritical() << "'bitmonerod' Daemon start failed. Is '" << daemon_handler.getDaemonProgram() << "' the daemon executable ?";
            this->onFatalError(2);
            return;
        }

        qDebug() << "DAEMON STARTED";

    }

    QObject::connect(&getinfo_timer, SIGNAL(timeout()), this, SLOT(getInfo()));
    getinfo_timer.start(5000);

    /* TODO : Move in another process (daemon itself ?) */
    QObject::connect(&savebc_timer, SIGNAL(timeout()), this, SLOT(saveBlockchain()));
    savebc_timer.start(1200000);

}
