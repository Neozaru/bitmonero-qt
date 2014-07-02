#ifndef METAINTERFACE_H
#define METAINTERFACE_H

#include <QObject>

class MetaInterface : public QObject {
    Q_OBJECT

public:
    virtual void enable() = 0;

protected:

    void onReady() {

        if ( !self_ready ) {
            self_ready = true;
            emit ready();
        }

    }

    void onFatalError(int pErrorCode) {
        emit fatalError(pErrorCode);
    }

signals:
    void ready();
    void fatalError(int pErrorCode);

private:

    bool self_ready = false;


};

#endif // METAINTERFACE_H
