#ifndef MONEROINTERFACE_HH
#define MONEROINTERFACE_HH

#include "Models/MoneroModel.h"
#include <ctype.h>

class MoneroInterface : public QObject {
    Q_OBJECT
public:
    MoneroInterface(MoneroModel& pMoneroModel) : monero_model(pMoneroModel), daemon_ready(false) {}
    virtual ~MoneroInterface() {}

//    virtual bool isReady() = 0;
    virtual int enable() = 0;

    virtual bool isOk() = 0;
//    virtual MoneroModel& getMoneroModel() = 0;

signals:
    void ready();

protected:
    void onInfoUpdated(unsigned int pBlockchainHeight, unsigned int pTargetBlockchainHeight, unsigned int pDifficulty, unsigned int pIncomingConnections, unsigned int pOutgoingConnections) {

        monero_model.setBlockchainHeight(pBlockchainHeight);
        monero_model.setTargetBlockchainHeight(pTargetBlockchainHeight);

    }

    void onReady() {

        if ( !daemon_ready ) {
            daemon_ready = true;
            emit ready();
        }

    }

protected:
    MoneroModel& monero_model;

    bool daemon_ready;
};

#endif // MONEROINTERFACE_HH
