#ifndef MINERMODEL_H
#define MINERMODEL_H

#include <QObject>
#include <QDebug>

#include "AbstractModel.h"

class MinerInterface;

class MinerModel : public AbstractModel
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ getEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(unsigned int nbThreads READ getNbThreads  WRITE setNbThreads NOTIFY nbThreadsChanged)
    Q_PROPERTY(unsigned int status READ getStatus  WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(double hashrate READ getHashrate  WRITE setHashrate NOTIFY hashrateChanged)
    Q_PROPERTY(QString address READ getAddress WRITE setAddress NOTIFY addressChanged)

public:

    enum miner_status_e {
        STATUS_UNKNOWN = -1,
        STATUS_OFF,
        STATUS_ON,
        STATUS_ERROR
    };

    MinerModel();

    void setMinerInterface(MinerInterface* iface)
    {
        miner_interface = iface;
    }

    unsigned int getNbThreads() const
    {
        return nbThreads;
    }

    unsigned int getStatus() const
    {
        return status;
    }

    double getHashrate() const
    {
        return hashrate;
    }

    bool getEnabled() const
    {
        return enabled;
    }

    const QString& getAddress() const
    {
        return address;
    }

public slots:


    void startMining();
    void stopMining();

    void optionChanged() {
        if (enabled && status == 0) {
            startMining();
        }
        else if ( status == 1 ) {
            stopMining();
        }
    }

    void setNbThreads(unsigned int pNbThreads)
    {
        if (nbThreads != pNbThreads) {
            nbThreads = pNbThreads;
            emit nbThreadsChanged(nbThreads);
        }
    }
    void setStatus(unsigned int pStatus)
    {
        if (status != pStatus) {
            status = pStatus;
            emit statusChanged(status);
        }
    }

    void setHashrate(double pHashrate)
    {
        if (hashrate != pHashrate) {
            hashrate = pHashrate;
            emit hashrateChanged(hashrate);
        }
    }

    void setEnabled(bool pEnabled)
    {
//        qDebug() << "Mining enabled toggled : " << pEnabled;

        if (enabled != pEnabled) {
            enabled = pEnabled;
            emit enabledChanged(enabled);
        }
    }

    void setAddress(const QString& pAddress)
    {

        qDebug() << "New address : " << pAddress;

        if (address != pAddress) {
            address = pAddress;
            emit addressChanged(address);
        }
    }

signals:
    void nbThreadsChanged(unsigned int pNbThreads);
    void statusChanged(unsigned int pStatus);
    void hashrateChanged(double pHashrate);
    void enabledChanged(bool enabled);
    void addressChanged(QString address);

private:
    MinerInterface* miner_interface;

    bool enabled;
    unsigned int nbThreads;
    unsigned int status;
    double hashrate;
    QString address;
};

#endif // MINERMODEL_H
