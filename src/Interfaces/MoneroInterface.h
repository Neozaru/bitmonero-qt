#ifndef MONEROINTERFACE_HH
#define MONEROINTERFACE_HH

#include "Models/MoneroModel.h"
#include <ctype.h>

class MoneroInterface : public QObject {
public:
    MoneroInterface(MoneroModel& pMoneroModel) : monero_model(pMoneroModel) {}

    virtual bool isReady() = 0;
//    virtual MoneroModel& getMoneroModel() = 0;

protected:
    void onInfoUpdated(unsigned int pBlockchainHeight, unsigned int pTargetBlockchainHeight, unsigned int pDifficulty, unsigned int pIncomingConnections, unsigned int pOutgoingConnections) {

        monero_model.setBlockchainHeight(pBlockchainHeight);
        monero_model.setTargetBlockchainHeight(pTargetBlockchainHeight);

    }

protected:
    MoneroModel& monero_model;
};

#endif // MONEROINTERFACE_HH
