#ifndef MONEROMODEL_HH
#define MONEROMODEL_HH

#include <QObject>

class MoneroModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int blocks_count READ getBlocksCount WRITE setBlocksCount NOTIFY blocksCountChanged)
public:
    MoneroModel();

    void setBlocksCount(unsigned int pCount) {
        if ( pCount == blocks_count ) {
            return;
        }

        blocks_count = pCount;

        /* Notify ui */
        blocksCountChanged();
    }

    unsigned int getBlocksCount() const {
        return blocks_count;
    }


signals:
    void blocksCountChanged();

private:
    unsigned int blocks_count;
};

#endif // MONEROMODEL_HH
