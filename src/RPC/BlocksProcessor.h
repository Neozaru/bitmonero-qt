#ifndef BLOCKSPROCESSOR_H
#define BLOCKSPROCESSOR_H

#include <QObject>
#include <functional>

#include "Common/Block.h"



class BlocksProcessor : public QObject
{
    Q_OBJECT
public:

    typedef std::function<void(unsigned long long, std::function<void(Block)>)> tBlockGetter;

    struct BlockInfo {
        unsigned int timestamp;
    };

    BlocksProcessor(tBlockGetter pBlockGetter);
    ~BlocksProcessor();

    bool saveBlocksInfo() const;
    bool loadBlocksInfo();

    int getBlockTimestamp(unsigned int pBlockIndex) const;
    bool isBlockProcessed(unsigned int pBlockIndex) const;


    void update(unsigned long long pNewHeight);

public slots:
    void processBlock(const Block& pBlock);

private slots:
    void onBlockProcessed(unsigned int pBlockIndex);

signals:
    void blockProcessed(unsigned int block_index);

private:

    void pullBlocks(unsigned long longpStartIndex, unsigned long long pEndIndex, unsigned int pParallelRequests = 1);
    void pullBlock(unsigned long long pIndex);

    unsigned int blocks_processed_from;
    unsigned int blocks_processed_to;

    std::map<unsigned int,BlockInfo> blocks_info;

    tBlockGetter blockGetter;

    bool block_pull_in_process;

};

#endif // BLOCKSPROCESSOR_H
