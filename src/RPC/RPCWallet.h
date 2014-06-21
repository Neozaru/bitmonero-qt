#ifndef RPCWALLET_HH
#define RPCWALLET_HH

#include <ctype.h>


#include <QJsonArray>
#include <QJsonObject>
#include <QTimer>

#include "Interfaces/WalletInterface.h"


#include "JsonRPCCommunicator.h"
#include "JsonRPCRequest.h"
#include "WalletSettings.h"

#include "Models/WalletModel.h"
#include "RPC/WalletHandlerProcess.h"

class RPCWallet : public WalletInterface
{
    Q_OBJECT
public:
    RPCWallet(WalletModel& pModel, const WalletSettings& pSettings);
    ~RPCWallet();

    void transfer(unsigned long long pAmount, const QString& pAddress, unsigned long long pFee, const QString& pPaymentId);
    void store();
    void getPayments(const QString& pPaymentId);

    int enable();


public slots:

    void getAddress();
    void getBalance();
    void getIncomingTransfers(const QString& pFilter = "all");

    void balanceResponse(const QJsonObject& pObjResponse);
    void transferResponse(const QJsonObject& pObjResponse, const QJsonObject& pObjOriginalParams);
    void addressResponse(const QJsonObject& pObjResponse);

signals:
    void walletReady();

private:

    const WalletSettings& settings;
    bool ready;

    bool should_spawn_wallet;

    JsonRPCCommunicator rpc;

    QTimer getbalance_timer;
    QTimer getaddress_timer;
    QTimer incomingtransfers_timer;
};

#endif // RPCWALLET_HH
