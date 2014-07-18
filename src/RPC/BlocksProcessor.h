#ifndef BLOCKSPROCESSOR_H
#define BLOCKSPROCESSOR_H

#include <QObject>

#include "Common/Block.h"


class BlocksProcessor : public QObject
{
    Q_OBJECT
public:

    struct BlockInfo {
        unsigned int timestamp;
    };

    BlocksProcessor();
    ~BlocksProcessor();

    bool saveBlocksInfo() const;
    bool loadBlocksInfo();

    int getBlockTimestamp(unsigned int pBlockIndex) const;
    bool isBlockProcessed(unsigned int pBlockIndex) const;

public slots:
    void processBlock(const Block& pBlock);

private slots:
    void onBlockProcessed(unsigned int pBlockIndex);

signals:
    void blockProcessed(unsigned int block_index);

private:

    unsigned int blocks_processed_from;
    unsigned int blocks_processed_to;

    std::map<unsigned int,BlockInfo> blocks_info;

};

#endif // BLOCKSPROCESSOR_H
