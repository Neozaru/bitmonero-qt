#include "WalletHandler.h"

#include <QStringList>
#include <QDebug>

WalletHandler::WalletHandler(const QString& pProgramPath)
    : open(false)
{
    mainProcess.setProgram(pProgramPath);

}

bool WalletHandler::openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort) {

    if ( mainProcess.state() == QProcess::Running ) {
        qWarning() << "Wallet is already open. Please close your wallet before opening another one";
        return false;
    }


    QStringList lArguments;

    lArguments.append("--wallet=" + pWalletFile);
    lArguments.append("--pass=" + pWalletPassword);
    lArguments.append("--rpc-bind-ip=" + pBindIP);
    lArguments.append("--rpc-bind-port=" + QString::number(pBindPort));

    mainProcess.setArguments(lArguments);

    mainProcess.start();

    /* Wait 3 seconds to check if program terminates (daemon not running, wrongp password ... */
    bool lFinished = mainProcess.waitForFinished(5000);
    qDebug() << "Finished ? " << lFinished;
    qDebug() << "State : " << mainProcess.state();

    if (lFinished) {
        return false;
    }

    open = true;

    return true;

}

bool WalletHandler::closeWallet() {

    open = false;

    if ( mainProcess.state() == QProcess::Running ) {
        mainProcess.kill();
        return true;
    }

    /* Was already closed */
    return false;

}

bool WalletHandler::createWallet(const QString& pFile, const QString& pPassword)
{

    if ( pFile.isEmpty() || pPassword.isEmpty() ) {
        qWarning() << "No file or password defined";
        return false;
    }

    QProcess lCreateWalletProcess;
    lCreateWalletProcess.setProgram(mainProcess.program());

    QStringList lArguments;

    lArguments.append("--generate-new-wallet=" + pFile);
    lArguments.append("--password="+ pPassword);
    lArguments.append("--exit-after-cmd=true");
    lCreateWalletProcess.setArguments(lArguments);

    lCreateWalletProcess.start();

    if ( !lCreateWalletProcess.waitForFinished(5000) ) {
        qWarning() << "createWallet failed : SubProcess doesn't responded";

        qWarning() << "Please ensure that your executable is located at : " << lCreateWalletProcess.program();

        return false;
    }

    int lExitCode = lCreateWalletProcess.exitCode();
    qDebug() << "PROCESS finished with : " << QString::number(lExitCode);

    return lExitCode == 0;
}
