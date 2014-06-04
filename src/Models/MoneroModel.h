#ifndef MONEROMODEL_HH
#define MONEROMODEL_HH

#include <QObject>

class MoneroInterface;

class MoneroModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int blocks_count READ getBlocksCount WRITE setBlocksCount NOTIFY blocksCountChanged)
    Q_PROPERTY(unsigned int blockchain_height READ getBlockchainHeight WRITE setBlockchainHeight NOTIFY blockchainHeightChanged)
    Q_PROPERTY(unsigned int target_blockchain_height READ getTargetBlockchainHeight WRITE setTargetBlockchainHeight NOTIFY targetBlockchainHeightChanged)

public:
    MoneroModel();

//    void setMoneroInterface(MoneroInterface* pMoneroInterface) {
//        monero_interface = pMoneroInterface;
//    }

    void setBlocksCount(unsigned int pCount) {
        if ( pCount == blocks_count ) {
            return;
        }

        blocks_count = pCount;

        /* Notify ui */
        blocksCountChanged();
    }

    unsigned int getBlocksCount() const {
        return blocks_count;
    }


    unsigned int getBlockchainHeight() const
    {
        return blockchain_height;
    }

    unsigned int getTargetBlockchainHeight() const
    {
        return target_blockchain_height;
    }

public slots:
    void setBlockchainHeight(unsigned int pBlockchainHeight)
    {
        if (blockchain_height != pBlockchainHeight) {
            blockchain_height = pBlockchainHeight;
            emit blockchainHeightChanged(pBlockchainHeight);
        }
    }

    void setTargetBlockchainHeight(unsigned int pTargetBlockchainHeight)
    {
        if (target_blockchain_height != pTargetBlockchainHeight) {
            target_blockchain_height = pTargetBlockchainHeight;
            emit targetBlockchainHeightChanged(pTargetBlockchainHeight);
        }
    }



signals:
    void blocksCountChanged();

    void blockchainHeightChanged(unsigned int arg);

    void targetBlockchainHeightChanged(unsigned int arg);

private:
    unsigned int blocks_count;
    unsigned int blockchain_height;
    unsigned int target_blockchain_height;

    MoneroInterface* monero_interface;
};

#endif // MONEROMODEL_HH
