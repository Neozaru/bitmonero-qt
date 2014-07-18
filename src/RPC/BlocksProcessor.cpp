#include "BlocksProcessor.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

QString gBlocksInfoFile("blocksinfo.json");

BlocksProcessor::BlocksProcessor()
    : blocks_processed_from(0), blocks_processed_to(0)
{
    QObject::connect(this, SIGNAL(blockProcessed(uint)), this, SLOT(onBlockProcessed(uint)));
    loadBlocksInfo();
}

BlocksProcessor::~BlocksProcessor()
{
    qDebug() << "[BP] Saving blocks info : " << saveBlocksInfo();
}

bool BlocksProcessor::saveBlocksInfo() const
{

    QJsonObject lJsonObject;

    QJsonObject lJsonBlocksInfo;
    for(std::pair<unsigned int, const BlockInfo&> lKeyValue : blocks_info) {
        QJsonObject lJsonBlkInfo;
        lJsonBlkInfo["timestamp"] = QJsonValue::fromVariant(lKeyValue.second.timestamp);
        lJsonBlocksInfo.insert(QString::number(lKeyValue.first), lJsonBlkInfo);
    }

    lJsonObject["blocks_info"] = lJsonBlocksInfo;
    QJsonDocument lJsonDoc;
    lJsonDoc.setObject(lJsonObject);

    QFile lFile(gBlocksInfoFile);

    if (!lFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write blocksinfo file.");
        return false;
    }

    lFile.write(lJsonDoc.toBinaryData());

    return true;
}

bool BlocksProcessor::loadBlocksInfo()
{
    QFile lFile(gBlocksInfoFile);

    if (!lFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open blocksinfo file.");
        return false;
    }

    QByteArray lRawData = lFile.readAll();

    const QJsonDocument lJsonData(QJsonDocument::fromBinaryData(lRawData));

    const QJsonObject& lJsonObject = lJsonData.object();

    const QJsonObject& lJsonBlocksInfo = lJsonObject["blocks_info"].toObject();


    for (const QString& lKey : lJsonBlocksInfo.keys()) {
        unsigned int lBlockIndex = lKey.toInt();
        const QJsonObject& lJsonBlkInfo = lJsonBlocksInfo[lKey].toObject();
        BlockInfo lBlockInfo;
        lBlockInfo.timestamp = lJsonBlkInfo["timestamp"].toInt(0);
        blocks_info[lBlockIndex] = lBlockInfo;
        onBlockProcessed(lBlockIndex);
        qDebug() << "Load blk " << lKey;
    }

    return true;

}

void BlocksProcessor::processBlock(const Block& pBlock)
{
    /* TODO : Check if async HTTP requests runs in threads or event loop */
    qDebug() << "Processing block " << QString::number(pBlock.height) << " with ts : " << QString::number(pBlock.timestamp);
    BlockInfo lBlockInfo;
    lBlockInfo.timestamp = pBlock.timestamp;
    blocks_info[pBlock.height] = lBlockInfo;


    emit blockProcessed(pBlock.height);
}


void BlocksProcessor::onBlockProcessed(unsigned int pBlockIndex)
{
    if (pBlockIndex < blocks_processed_from) {
        blocks_processed_from = pBlockIndex;
    }

    if (pBlockIndex > blocks_processed_to) {
        blocks_processed_to = pBlockIndex;
    }
}


int BlocksProcessor::getBlockTimestamp(unsigned int pBlockIndex) const {

    if(blocks_info.find(pBlockIndex) == blocks_info.end()) {
        return -1;
    }

    return blocks_info.at(pBlockIndex).timestamp;
}

bool BlocksProcessor::isBlockProcessed(unsigned int pBlockIndex) const
{
    return blocks_info.find(pBlockIndex) != blocks_info.end();
}
