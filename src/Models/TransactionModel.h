#ifndef TRANSACTIONMODEL_H
#define TRANSACTIONMODEL_H

#include <QObject>
#include <QDateTime>

#include "Common/Transaction.h"

class TransactionModel;

class TransactionModel : public QObject
{
    Q_OBJECT


public:

    TransactionModel(const TransactionModel& pTransactionModel)
        : QObject(), block_height(pTransactionModel.getBlockHeight()), id(pTransactionModel.getId()), amount(pTransactionModel.getAmount()), spendable(pTransactionModel.isSpendable()), type(pTransactionModel.getType()), date(pTransactionModel.getDate())
    {

    }

    TransactionModel(unsigned long pBlockHeight, const QString& pId, double pAmount, bool pSpendable, bool pType, const QDateTime& pDate = QDateTime())
        :  block_height(pBlockHeight), id(pId), amount(pAmount), spendable(pSpendable), type(pType), date(pDate)
    {

    }

    TransactionModel(const Transaction& pTransaction)
        : block_height(pTransaction.block_height), id(pTransaction.hash), amount(pTransaction.amount), spendable(pTransaction.spendable)
    {

    }

    Q_PROPERTY(unsigned long long block_height READ getBlockHeight NOTIFY blockHeightChanged)
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(double amount READ getAmount NOTIFY amountChanged)
    Q_PROPERTY(bool spendable READ isSpendable NOTIFY spendableChanged)
    Q_PROPERTY(bool type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QDateTime date READ getDate NOTIFY dateChanged)

    QString getId() const
    {
        return id;
    }

    double getAmount() const
    {
        return amount;
    }

    bool isSpendable() const
    {
        return spendable;
    }

    bool getType() const
    {
        return type;
    }

    unsigned long long getBlockHeight() const
    {
        return block_height;
    }

    void setAmount(double pAmount) {
        amount = pAmount;
    }

    void setSpendable(bool pSpendable) {
        spendable = pSpendable;
    }

    const QDateTime& getDate() const
    {
        return date;
    }

    bool operator<(const TransactionModel& pOther)
    {
        return block_height < pOther.getBlockHeight();
    }


    static bool dereferencedLessThan(QObject* o1, QObject* o2) {
        return *dynamic_cast<TransactionModel*>(o2) < *dynamic_cast<TransactionModel*>(o1);
    }

signals:

    void idChanged(QString arg);

    void amountChanged(double arg);

    void spendableChanged(bool arg);

    void typeChanged(bool arg);

    void blockHeightChanged(unsigned long long arg);

    void dateChanged(const QDateTime& arg);

public slots:

private:

    unsigned long long block_height;

    QString id;

    double amount;

    bool spendable;

    bool type;

    QDateTime date;
};

#endif // TRANSACTIONMODEL_H
