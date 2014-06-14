#ifndef TRANSACTIONMODEL_H
#define TRANSACTIONMODEL_H

#include <QObject>

class TransactionModel : public QObject
{
    Q_OBJECT


public:
//    explicit TransactionModel(QObject *parent = 0);

    TransactionModel(const TransactionModel& pTransactionModel)
        : TransactionModel(pTransactionModel.getId(), pTransactionModel.getAmount(), pTransactionModel.isSpendable(), pTransactionModel.getType())
    {

    }

    TransactionModel(const QString& pId, double pAmount, bool pSpendable, bool pType)
        :  id(pId), amount(pAmount), spendable(pSpendable), type(pType)
    {

    }

    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(double amount READ getAmount NOTIFY amountChanged)
    Q_PROPERTY(bool spendable READ isSpendable NOTIFY spendableChanged)
    Q_PROPERTY(bool type READ getType NOTIFY typeChanged)

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

    void setAmount(double pAmount) {
        amount = pAmount;
    }

    void setSpendable(bool pSpendable) {
        spendable = pSpendable;
    }

signals:

    void idChanged(QString arg);

    void amountChanged(double arg);

    void spendableChanged(bool arg);

    void typeChanged(bool arg);

public slots:

private:

    QString id;

    double amount;

    bool spendable;

    bool type;

};

#endif // TRANSACTIONMODEL_H
