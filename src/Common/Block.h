#ifndef BLOCK_H
#define BLOCK_H

#include <QJsonObject>

class Block
{
public:
    Block();

    static Block fromJson(const QJsonObject& pJson);

    unsigned long long height;
    unsigned int timestamp;


};

#endif // BLOCK_H
