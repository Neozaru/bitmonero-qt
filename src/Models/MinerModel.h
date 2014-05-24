#ifndef MINERMODEL_H
#define MINERMODEL_H

#include <QObject>
#include <QDebug>

class MinerModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY toggled)
    Q_PROPERTY(unsigned int nbThreads READ getNbThreads  WRITE setNbThreads NOTIFY nbThreadsChanged)
    Q_PROPERTY(unsigned int status READ getStatus  WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(double hashrate READ getHashrate  WRITE setHashrate NOTIFY hashrateChanged)
    Q_PROPERTY(QString address READ getAddress WRITE setAddress NOTIFY addressChanged)

public:
    MinerModel();
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

    bool isEnabled() const
    {
        return enabled;
    }

    QString getAddress() const
    {
        return address;
    }

public slots:

    void startMining() {



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
        qDebug() << "Toggled : " << pEnabled;

        if (enabled != pEnabled) {
            enabled = pEnabled;
            emit toggled(enabled);
        }
    }

    void setAddress(QString pAddress)
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
    void toggled(bool pEnabled);
    void addressChanged(QString pAddress);

private:
    unsigned int nbThreads;
    unsigned int status;
    double hashrate;
    bool enabled;
    QString address;
};

#endif // MINERMODEL_H
