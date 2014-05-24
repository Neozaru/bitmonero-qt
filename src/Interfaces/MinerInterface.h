#ifndef MINERINTERFACE_H
#define MINERINTERFACE_H

#include "Models/MinerModel.h"

class MinerInterface {

public:
    MinerInterface(MinerModel& pMinerModel) : miner_model(pMinerModel) { pMinerModel.setMinerInterface(this);}

    virtual void startMining(const QString& pMoneroAddress, unsigned int pNbThreads = 1) = 0;
    virtual void stopMining() = 0;


protected:

    void onStartMiningSucceeded() {
        miner_model.setStatus(1);
    }

    void onStartMiningFailed(const QString& pReason) {
        miner_model.setEnabled(false);
    }

    void onStopMiningSucceeded() {
        miner_model.setStatus(0);
    }

    void onStopMiningFailed(const QString& pReason) {
        miner_model.setStatus(0);
        miner_model.setEnabled(false);
    }

private:
    MinerModel& miner_model;
};

#endif // MINERINTERFACE_H
