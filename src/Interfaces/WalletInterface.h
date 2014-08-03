#ifndef WALLETINTERFACE_HH
#define WALLETINTERFACE_HH

#include <QDebug>
#include <QList>

#include <map>
#include <ctype.h>

#include "MetaInterface.h"
#include "Models/WalletModel.h"
#include "Models/TransactionModel.h"
#include "Interfaces/MoneroInterface.h"
#include "Common/Transaction.h"
/**
 * @brief Handles a wallet instance.
 * Provides wallet-specific methods for interacting with an existing wallet. (balance, transfers, etc)
 * In RPC mode, 'enable()' method will only try to connect to RPC wallet already instanciated by WalletHandlerInterface
 * In LibMoneroWallet mode, 'enable()' method will handle wallet opening
 */
class WalletInterface : public MetaInterface
{

public:
    WalletInterface(WalletModel& pWalletModel, const MoneroInterface& pMoneroInterface) : wallet_model(pWalletModel), monero_interface(pMoneroInterface) {
        pWalletModel.setWalletInterface(this);
    }

    virtual ~WalletInterface() {}

    virtual void getBalance() = 0;
    virtual void getAddress() = 0;
    virtual void transfer(unsigned long long pAmount, const QString& pAddress, unsigned long long pFee, const QString& pPaymentId, int pMixinCount) = 0;
    virtual void getPayments(const QString& pPaymentId) = 0;
    virtual void getIncomingTransfers(const QString& pFilter) = 0;
    virtual void store() = 0;

protected:

    void onBalanceUpdated(double pBalance, double pUnlockedBalance)
    {
        wallet_model.setReady(true);
        wallet_model.setBalance(pBalance);
        wallet_model.setUnlockedBalance(pUnlockedBalance);
    }


    void onAddressUpdated(const QString& pAddress)
    {
        wallet_model.setReady(true);
        wallet_model.setAddress(pAddress);
    }


    void onTransferSuccessful(const QString& pTxHash, unsigned long long pAmount, const QString& pAddress, unsigned long long pFee)
    {
        emit wallet_model.transferSuccessful(pTxHash,pAmount,pAddress,pFee);
        qDebug() << "Transfer Successful : " << pAmount << " to " << pAddress << " (fee : " << pFee << ")\nHash: " << pTxHash;
    }


    void onIncomingTransfersUpdated(const QList<Transaction>& pTransactions) {

        QList<QObject*> lAbstractTransfersList;
        for(const Transaction& lTransaction : pTransactions) {
            lAbstractTransfersList.append(new TransactionModel(lTransaction));
        }

        wallet_model.setTransactions(lAbstractTransfersList);
        qDebug() << "Transactions SET";

        /* Aggregates incoming transactions */
        std::map<QString,TransactionModel*> lIncomingTransactionsMap = aggregateIncomingTransactions(pTransactions);


        QList<QObject*> lAbstractAggregatedIncomingTransfersList;
        for ( std::pair<QString,TransactionModel*> lAggregatedTransaction : lIncomingTransactionsMap) {
            lAbstractAggregatedIncomingTransfersList.append(lAggregatedTransaction.second);
        }

        qSort(lAbstractAggregatedIncomingTransfersList.begin(), lAbstractAggregatedIncomingTransfersList.end(), TransactionModel::dereferencedLessThan);

        wallet_model.setAggregatedIncomingTransactions(lAbstractAggregatedIncomingTransfersList);



        /* Aggregates outgoing transactions */
        std::map<unsigned long long,TransactionModel*> lOutgoingTransactionsMap = aggregateOutgoingTransactions(pTransactions);


        QList<QObject*> lAbstractAggregatedOutgoingTransfersList;
        for ( std::pair<unsigned long long,TransactionModel*> lAggregatedTransaction : lOutgoingTransactionsMap) {
            lAbstractAggregatedOutgoingTransfersList.append(lAggregatedTransaction.second);
        }

        qSort(lAbstractAggregatedOutgoingTransfersList.begin(), lAbstractAggregatedOutgoingTransfersList.end(), TransactionModel::dereferencedLessThan);

        wallet_model.setAggregatedOutgoingTransactions(lAbstractAggregatedOutgoingTransfersList);

    }


    void onTransferError(int pErrorCode, const QString& pErrorMessage) {

        qWarning() << "Transfer error " << pErrorCode << " : " << pErrorMessage;

        /* Throws to UI */
        emit wallet_model.transferError(pErrorCode, pErrorMessage);

    }

private:
    std::map<QString,TransactionModel*> aggregateIncomingTransactions(const QList<Transaction>& pTransactions)
    {
        std::map<QString,TransactionModel*> lTransactionsMap;
        for (const Transaction& lTransaction : pTransactions) {

            if (lTransactionsMap.count(lTransaction.hash) == 0) {
                lTransactionsMap[lTransaction.hash] = new TransactionModel(
                                                        lTransaction.block_height, lTransaction.hash, 0, true, false, monero_interface.getBlockDateTime(lTransaction.block_height));
            }

            TransactionModel* lAggregatedTransaction = lTransactionsMap[lTransaction.hash];
            lAggregatedTransaction->setAmount(lAggregatedTransaction->getAmount() + lTransaction.amount);
            lAggregatedTransaction->setSpendable(lAggregatedTransaction->isSpendable() && lTransaction.spendable);
        }

        return lTransactionsMap;
    }

    std::map<unsigned long long,TransactionModel*> aggregateOutgoingTransactions(const QList<Transaction>& pTransactions)
    {
        std::map<unsigned long long,TransactionModel*> lTransactionsMap;
        for (const Transaction& lTransaction : pTransactions) {

            if (!lTransaction.spendable && lTransaction.spent_block_height != 0) {

                if (lTransactionsMap.count(lTransaction.spent_block_height) == 0) {
                    lTransactionsMap[lTransaction.spent_block_height] = new TransactionModel(
                                                            lTransaction.spent_block_height, "", 0, true, false, monero_interface.getBlockDateTime(lTransaction.spent_block_height));
                }

                TransactionModel* lAggregatedTransaction = lTransactionsMap[lTransaction.spent_block_height];
                lAggregatedTransaction->setAmount(lAggregatedTransaction->getAmount() + lTransaction.amount);
                lAggregatedTransaction->setSpendable(false);
            }

        }

        return lTransactionsMap;
    }

    WalletModel& wallet_model;

    const MoneroInterface& monero_interface;
};

#endif // WALLETINTERFACE_HH
