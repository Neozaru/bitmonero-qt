#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QObject>

class AbstractModel : public QObject {

    Q_OBJECT
    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)


public:

    AbstractModel();
    ~AbstractModel();

    bool isReady() const
    {
        return ready;
    }

    void setReady(bool pReady) {

        if ( ready != pReady ) {
            ready = pReady;
            emit readyChanged(ready);
        }

    }

signals:
    void readyChanged(bool ready);

private:
    bool ready;
};

#endif // ABSTRACTMODEL_H
