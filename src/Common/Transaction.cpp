#include "Transaction.h"

#include <exception>
#include <stdexcept>

Transaction::Transaction(unsigned long long pBlockHeight, const QString& pHash, double pAmount, bool pSpendable)
    : block_height(pBlockHeight), hash(pHash), amount(pAmount), spendable(pSpendable)
{
}


Transaction Transaction::fromJson(const QJsonObject& pJsonObject)
{
    if (!pJsonObject["tx_hash"].isString() ||
            !pJsonObject["amount"].isDouble() || !pJsonObject["spent"].isBool()) {
        throw(std::invalid_argument("Invalid json transaction format"));
    }

    return Transaction(
            pJsonObject["block_height"].toInt(0),
            pJsonObject["tx_hash"].toString(),
            pJsonObject["amount"].toDouble(),
            pJsonObject["spent"].toBool()
    );
}
