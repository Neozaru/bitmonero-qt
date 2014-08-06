#ifndef RPCMONERO_H
#define RPCMONERO_H


#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QTimer>


#include "Models/MoneroModel.h"
#include "Interfaces/MoneroInterface.h"

#include "JsonRPCCommunicator.h"

#include "DaemonHandler.h"
#include "BlocksProcessor.h"

#include "Common/Block.h"

class RPCMonero : public MoneroInterface
{
    Q_OBJECT
public:


    RPCMonero(MoneroModel& pMoneroModel, const WalletSettings& pSettings);
    ~RPCMonero();

    QDateTime getBlockDateTime(unsigned int pBlockIndex) const;

    void getBlockInfo(unsigned long long pBlockIndex);

    void enable();

public slots:
    void getInfo();

//    TODO : Move this in another process
    void saveBlockchain();

private:

//    void pullBlockByWindow(unsigned int pStartIndex = 1, unsigned int pEndIndex = 0, unsigned int pWindowSize = 10);
//    void pullBlocks(unsigned long longpStartIndex, unsigned long long pEndIndex, unsigned int pParallelRequests = 1);
//    void pullBlock(unsigned long long pIndex);

    void processBlock(const Block& pBlock);


    void blockchainHeightUpdated(unsigned int pNewHeight);

    void getBlock(unsigned long long pIndex, std::function<void(Block)> pCallback);


    DaemonHandler daemon_handler;

    bool should_spawn_daemon;

    JsonRPCCommunicator rpc;

    QTimer getinfo_timer;
    QTimer savebc_timer;

    BlocksProcessor blocks_processor;

    unsigned int blockchain_height;
//    unsigned int last_pulled_block;

//    bool block_pull_in_process;

};

#endif // RPCMONERO_H
