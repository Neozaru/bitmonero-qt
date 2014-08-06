#ifndef MONEROINTERFACE_HH
#define MONEROINTERFACE_HH

#include <ctype.h>
#include <QDateTime>

#include "MetaInterface.h"
#include "Models/MoneroModel.h"
#include "Common/Block.h"

/**
 * @brief Handles Monero/Daemon interface.
 * Basically, this class will handle all actions and events related to 'bitmonerod'
 */
class MoneroInterface : public MetaInterface {
    Q_OBJECT
public:
    MoneroInterface(MoneroModel& pMoneroModel) : monero_model(pMoneroModel) {}
    virtual ~MoneroInterface() {}

    virtual QDateTime getBlockDateTime(unsigned int pBlockIndex) const = 0;

    virtual void getBlockInfo(unsigned long long pBlockIndex) = 0;

signals:
    void blockInfoReceived(Block blockInfo);

protected:
    void onInfoUpdated(unsigned int pBlockchainHeight, unsigned int pTargetBlockchainHeight, unsigned int pDifficulty, unsigned int pIncomingConnections, unsigned int pOutgoingConnections) {

        monero_model.setBlockchainHeight(pBlockchainHeight);
        monero_model.setTargetBlockchainHeight(pTargetBlockchainHeight);

    }

    void onBlockInfoReceived(const Block& pBlockInfo) {
        emit blockInfoReceived(pBlockInfo);
    }

protected:
    MoneroModel& monero_model;

};

#endif // MONEROINTERFACE_HH
