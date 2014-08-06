#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QJsonObject>

class Transaction
{
public:
    Transaction(unsigned long long pBlockHeight, const QString& pHash, double pAmount, bool pSpendable, unsigned long long pSpentBlockHeight = 0);

    static Transaction fromJson(const QJsonObject& pJsonObject);


public:

    unsigned long long block_height;
    QString hash;
    double amount;
    bool spendable;
    unsigned long long spent_block_height;
};

#endif // TRANSACTION_H
