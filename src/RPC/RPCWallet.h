#ifndef RPCWALLET_HH
#define RPCWALLET_HH

#include <QJsonArray>
#include <QJsonObject>

#include "Interfaces/WalletInterface.h"


#include "JsonRPCCommunicator.h"
#include "JsonRPCRequest.h"
//#include "Models/WalletModel.h"

class RPCWallet :  public QObject, public WalletInterface
{
    Q_OBJECT
public:
    RPCWallet(WalletModel& pModel, const QString& pHost, unsigned int pPort);

    void transfer(double pAmount, const QString& pAddress, int pFee, const QString& pPaymentId);
    void store();
    void getPayments(const QString& pPaymentId);



public slots:

    void getAddress();
    void getBalance();

    void balanceResponse(const QJsonObject& pObjResponse);
    void transferResponse(const QJsonObject& pObjResponse, const QJsonObject& pObjOriginalParams);
    void addressResponse(const QJsonObject& pObjResponse);

private:
    JsonRPCCommunicator rpc;
};

#endif // RPCWALLET_HH
