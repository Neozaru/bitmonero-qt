#include "RPCMonero.h"

#include <memory>

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
    : MoneroInterface(pMoneroModel), daemon_handler(pSettings), rpc(pSettings.getMoneroUri(),pSettings.getMoneroPort()), blockchain_height(0),
      blocks_processor(std::bind(&RPCMonero::getBlock, this, std::placeholders::_1, std::placeholders::_2))
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

            unsigned int lBlockChainHeight = pJsonResponse["height"].toInt();

            blockchainHeightUpdated(lBlockChainHeight);

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

QDateTime RPCMonero::getBlockDateTime(unsigned int pBlockIndex) const {

    if (!blocks_processor.isBlockProcessed(pBlockIndex)) {
        return QDateTime();
    }

    return QDateTime::fromTime_t(blocks_processor.getBlockTimestamp(pBlockIndex));
}

void RPCMonero::saveBlockchain()
{

    JsonRPCRequest* lReq = rpc.sendRequest("save_bc",QJsonObject(), true);
    QObject::connect(lReq, &JsonRPCRequest::jsonResponseReceived,[this](const QJsonObject pJsonResponse) {
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


void RPCMonero::blockchainHeightUpdated(unsigned int pNewHeight)
{
    blockchain_height = pNewHeight;

    blocks_processor.update(blockchain_height-2);

}


void RPCMonero::getBlock(unsigned long long pIndex, std::function<void(Block)> pCallback) {

    qDebug() << "GET block " << pIndex;

    QJsonObject lParams;
    lParams["height"] = QJsonValue::fromVariant(pIndex);
    JsonRPCRequest* lReq = rpc.sendRequest("getblockheaderbyheight",lParams);
    QObject::connect(lReq, &JsonRPCRequest::jsonResponseReceived,[this, pIndex, pCallback](const QJsonObject pJsonResponse) {

        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {

            try {
                const Block lBlock = Block::fromJson(pJsonResponse);
//                blocks_processor.processBlock(lBlock);
                qDebug() << "RECV block " << pIndex << "/" << lBlock.height;
                pCallback(lBlock);
            }
            catch(std::runtime_error e) {
                qCritical() << "Block pull aborted.";
            }


        }
        else {
            qWarning() << "Bad status for 'getblockheaderbyheight' (" << QString::number(pIndex) << ") : " << lStatus;

        }
    });


    QObject::connect(this, &MoneroInterface::blockInfoReceived, [pIndex, pCallback](Block pBlockInfo) {
        qDebug() << "RECV block " << pIndex << "/" << pBlockInfo.height;
        pCallback(pBlockInfo);
    });
}

void RPCMonero::getBlockInfo(unsigned long long pBlockIndex)
{
    getBlock(pBlockIndex, [this](Block pBlockInfo){
        this->onBlockInfoReceived(pBlockInfo);
    });

}


