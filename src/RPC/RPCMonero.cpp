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
    : MoneroInterface(pMoneroModel), daemon_handler(pSettings), rpc(pSettings.getMoneroUri(),pSettings.getMoneroPort()), blockchain_height(0), last_pulled_block(0), block_pull_in_process(false)
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
    int lDifference = pNewHeight - blockchain_height;

//    TODO: For debug
    if (lDifference < 1) {
        return;
    }

    qDebug() << "New blockchain height : " << pNewHeight << ". Difference : " << lDifference;

    if (lDifference <= 1) {
        return;
    }


    /* If high difference, launch parallel processing */
    if (lDifference >= 100) {
        pullBlocks(blockchain_height, pNewHeight-1, 10);
    }
    else {
        pullBlocks(blockchain_height, pNewHeight-1);
    }

    blockchain_height = pNewHeight;
}

void RPCMonero::pullBlock(unsigned int pIndex)
{

    if (blocks_processor.isBlockProcessed(pIndex)) {
        qDebug() << "Block already processed" << pIndex;
        emit blocks_processor.blockProcessed(pIndex);
        return;
    }

    QJsonObject lParams;
    lParams["height"] = QJsonValue::fromVariant(pIndex);
    JsonRPCRequest* lReq = rpc.sendRequest("getblockheaderbyheight",lParams);
    QObject::connect(lReq, &JsonRPCRequest::jsonResponseReceived,[this, lReq, pIndex](const QJsonObject pJsonResponse) {

        const QString& lStatus = pJsonResponse["status"].toString();

        if ( lStatus == "OK" ) {

            try {
                const Block lBlock = Block::fromJson(pJsonResponse);
                blocks_processor.processBlock(lBlock);
            }
            catch(std::runtime_error e) {
                qCritical() << "Block pull aborted.";
            }


        }
        else {
            qWarning() << "Bad status for 'getblockheaderbyheight' (" << QString::number(pIndex) << ") : " << lStatus;

        }
    });
}


void RPCMonero::pullBlocks(unsigned int pStartIndex, unsigned int pEndIndex, unsigned int pParallelRequests)
{

    if (block_pull_in_process) {
        qDebug() << "Blocks processing already in process";
        return;
    }

    if (pParallelRequests < 1) {
        pParallelRequests = 1;
    }

    if (pEndIndex < pStartIndex) {
        qWarning() << "Asked block pull from " << pStartIndex << " to " << pEndIndex << ". Aborting.";
        return;
    }

    block_pull_in_process = true;
    auto lNextWindowCallback = [this, pEndIndex, pParallelRequests] (unsigned int pBlockIndex) {
        if(pBlockIndex < pEndIndex) {
            pullBlock(pBlockIndex+pParallelRequests);
        }
        else {
            block_pull_in_process = false;
        }
    };
    QObject::connect(&blocks_processor, &BlocksProcessor::blockProcessed, lNextWindowCallback);

    for (unsigned int i=0; i<pParallelRequests; i++) {
        pullBlock(pStartIndex+i);
    }


}


