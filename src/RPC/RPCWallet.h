#ifndef RPCWALLET_HH
#define RPCWALLET_HH

#include <QJsonArray>
#include <QJsonObject>

#include "Interfaces/WalletInterface.h"


#include "JsonRPCCommunicator.h"
#include "JsonRPCRequest.h"
#include "WalletSettings.h"

#include "Models/WalletModel.h"

class RPCWallet : public WalletInterface
{
    Q_OBJECT
public:
    RPCWallet(WalletModel& pModel, const WalletSettings& pSettings);

    void transfer(double pAmount, const QString& pAddress, int pFee, const QString& pPaymentId);
    void store();
    void getPayments(const QString& pPaymentId);


    void enable();


public slots:

    void getAddress();
    void getBalance();
    void getIncomingTransfers(const QString& pType = "all");

    void balanceResponse(const QJsonObject& pObjResponse);
    void transferResponse(const QJsonObject& pObjResponse, const QJsonObject& pObjOriginalParams);
    void addressResponse(const QJsonObject& pObjResponse);

signals:
    void walletReady();

private:
    JsonRPCCommunicator rpc;
    bool ready;
};

#endif // RPCWALLET_HH
