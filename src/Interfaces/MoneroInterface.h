#ifndef MONEROINTERFACE_HH
#define MONEROINTERFACE_HH

#include "Models/WalletModel.h"

class MoneroInterface : public QObject {
public:
    virtual bool isReady() = 0;
    virtual WalletModel& getWalletModel() = 0;
};

#endif // MONEROINTERFACE_HH
