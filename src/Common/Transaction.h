#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QJsonObject>

class Transaction
{
public:
    Transaction(unsigned long long pBlockHeight, const QString& pHash, double pAmount, bool pSpendable);

    static Transaction fromJson(const QJsonObject& pJsonObject);


public:

    unsigned long long block_height;
    QString hash;

    double amount;

    bool spendable;

};

#endif // TRANSACTION_H
