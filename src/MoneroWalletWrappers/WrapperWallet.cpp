#include "WrapperWallet.h"

#include <exception>
#include <QList>
#include <QtConcurrent/QtConcurrent>

#include "MoneroWallet/MoneroWallet.hh"
#include "MoneroWallet/MoneroErrors.hh"

#include "Models/TransactionModel.h"


WrapperWallet::~WrapperWallet() {

    if (wallet) {
        refresh_timer.stop();
        qDebug() << "Storing wallet data...";
        wallet->store();
        qDebug() << "Done.";
    }

}

int WrapperWallet::enable() {

    /* TODO : Review errors */
    try {
        wallet = new Monero::Wallet(settings.getWalletFile().toStdString(), settings.getWalletPassword().toStdString());
    }
    catch(Monero::Errors::InvalidFile e) {
        return 5;
    }
    catch(Monero::Errors::InvalidPassword e) {
        return 6;
    }

    const QString lMoneroAddressPort = settings.getMoneroUri() + ":" + QString::number(settings.getMoneroPort());

    qDebug() << "Connecting to : " << lMoneroAddressPort;

    wallet->connect(lMoneroAddressPort.toStdString());


    if ( !refreshWallet() ) {
        return 7;
    }

    QObject::connect(&refresh_timer,SIGNAL(timeout()), this, SLOT(refreshWallet()));
    refresh_timer.start(10000);


    emit this->onReady();

    return 0;
}

void WrapperWallet::getBalance()
{
    this->onBalanceUpdated( wallet->getBalanceMini(), wallet->getUnlockedBalanceMini() );
}

void WrapperWallet::getAddress()
{
    this->onAddressUpdated( QString::fromStdString(wallet->getAddress()) );
}

void WrapperWallet::transfer(unsigned long long pAmount, const QString& pAddress, unsigned long long pFee, const QString& pPaymentId) {

    QtConcurrent::run([this, pAmount, pAddress, pFee, pPaymentId]() {
        qDebug() << "Performing transfer of " << QString::number(pAmount) << " with fees : " << QString::number(pFee);

        try {
            const std::string& lTxHash = wallet->transferMini(pAddress.toStdString(), pAmount, pFee, pPaymentId.toStdString());
    //        const QString lTxHash = QString::fromStdString();
            this->onTransferSuccessful(QString::fromStdString(lTxHash), pAmount, pAddress, pFee);
            refreshWallet();
        }
        catch(const std::exception& e) {
            qDebug() << "Transfer error " << QString::fromStdString(e.what());
            this->onTransferError(-1, QString::fromStdString(e.what()));
        }

    });
}

void WrapperWallet::getPayments(const QString& pPaymentId) {

}


void WrapperWallet::getIncomingTransfers(const QString& pFilter) {
    const std::vector<Monero::Transfer>& lTransfers = wallet->getIncomingTransfers();

    QList<TransactionModel*> lTransfersModels;
    for ( Monero::Transfer lTransfer : lTransfers ) {
        lTransfersModels.append(new TransactionModel(QString::fromStdString(lTransfer.transaction_id), lTransfer.amount_mini, !lTransfer.spent, false));
    }

    this->onIncomingTransfersUpdated(lTransfersModels);
}


void WrapperWallet::store() {
    wallet->store();
}


bool WrapperWallet::refreshWallet() {

    qDebug() << "Refreshing wallet...";

    wallet->refresh();

    getAddress();
    getBalance();
    getIncomingTransfers();

    qDebug() << "Done Refreshing";

    return true;

}
