#include "Block.h"

#include <exception>

#include <QJsonDocument>
#include <QDebug>

Block::Block()
{
}


Block Block::fromJson(const QJsonObject& pJson)
{
    if (!pJson["block_header"].isObject()) {
        qCritical() << "Received bad block response format : " << pJson;
        throw(std::runtime_error("Invalid Json response"));
    }
    QJsonObject lBlockHeader = pJson["block_header"].toObject();

    if (!lBlockHeader["height"].isDouble() || !lBlockHeader["timestamp"].isDouble()) {
        qCritical() << "Received bad block header format : " << lBlockHeader;
        throw(std::runtime_error("Invalid Json response"));
    }

    Block lBlock;
    lBlock.height = lBlockHeader["height"].toInt();
    lBlock.timestamp = lBlockHeader["timestamp"].toInt();

    return lBlock;

}
