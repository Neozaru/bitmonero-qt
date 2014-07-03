#ifndef MONEROINTERFACE_HH
#define MONEROINTERFACE_HH

#include <ctype.h>

#include "MetaInterface.h"
#include "Models/MoneroModel.h"

/**
 * @brief Handles Monero/Daemon interface.
 * Basically, this class will handle all actions and events related to 'bitmonerod'
 */
class MoneroInterface : public MetaInterface {

public:
    MoneroInterface(MoneroModel& pMoneroModel) : monero_model(pMoneroModel) {}
    virtual ~MoneroInterface() {}


protected:
    void onInfoUpdated(unsigned int pBlockchainHeight, unsigned int pTargetBlockchainHeight, unsigned int pDifficulty, unsigned int pIncomingConnections, unsigned int pOutgoingConnections) {

        monero_model.setBlockchainHeight(pBlockchainHeight);
        monero_model.setTargetBlockchainHeight(pTargetBlockchainHeight);

    }


protected:
    MoneroModel& monero_model;

};

#endif // MONEROINTERFACE_HH
