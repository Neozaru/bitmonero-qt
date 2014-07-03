#ifndef METAINTERFACE_H
#define METAINTERFACE_H

#include <QObject>

/**
 * @brief Base Interface for Monero, Wallet, WalletHandler and Miner interfaces.
 * Exposes 'enable' method and resulting signals.
 * Each subInterface will be associated with a Model (Models are user-interaction entry points)
 */
class MetaInterface : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Common to all interfaces. This function should be called once at application startup.
     * Result is asynchronous : If enabling succeeds, "ready" signal is triggered, otherwise "fatalError" is triggered with error code.
     */
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
    /**
     * @brief Triggered when "enable()" succeeds
     */
    void ready();

    /**
     * @brief Triggered when "enable()" fails
     * @param pErrorCode Error code
     */
    void fatalError(int pErrorCode);

private:

    /**
     * @brief To prevent double "ready()" signal sending
     */
    bool self_ready = false;


};

#endif // METAINTERFACE_H
