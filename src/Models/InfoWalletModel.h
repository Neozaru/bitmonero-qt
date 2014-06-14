#ifndef INFOWALLETMODEL_H
#define INFOWALLETMODEL_H

#include <QObject>

class InfoWalletModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY addressChanged)
    Q_PROPERTY(int status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(QString file_path READ getFilePath NOTIFY filePathChanged)


public:
//    explicit InfoWalletModel(QObject *parent = 0);

    InfoWalletModel(const QString& pName, const QString& pPath, const QString& pAddress, int pStatus);

    const QString& getName() const
    {
        return name;
    }

    const QString& getAddress() const
    {
        return address;
    }

    int getStatus() const
    {
        return status;
    }

    const QString& getFilePath() const
    {
        return file_path;
    }

signals:

    void nameChanged(QString arg);

    void addressChanged(QString arg);

    void statusChanged(int arg);

    void filePathChanged(QString arg);

public slots:

private:

    QString name;
    QString file_path;
    QString address;
    int status;

};

#endif // INFOWALLETMODEL_H
