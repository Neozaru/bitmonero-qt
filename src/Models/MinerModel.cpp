#include "MinerModel.h"

#include <stdexcept>

#include "Interfaces/MinerInterface.h"


MinerModel::MinerModel()
    : miner_interface(NULL), enabled(false), nbThreads(1), status(0)
{
    QObject::connect(this,SIGNAL(enabledChanged(bool)),this,SLOT(optionChanged()));
}


void MinerModel::startMining() {
    if ( !miner_interface ) {
        throw std::logic_error("Miner Interface not set");
    }
    miner_interface->startMining(address,nbThreads);
}


void MinerModel::stopMining() {
    if ( !miner_interface ) {
        throw std::logic_error("Miner Interface not set");
    }
    miner_interface->stopMining();
}
