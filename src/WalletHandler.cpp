#include "WalletHandler.h"

#include <QStringList>
#include <QDebug>

//WalletHandler::WalletHandler()
//    : open(false)
//{
////    mainProcess.setProgram(pProgramPath);

//}

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

bool WalletHandler::walletFileExists(const QString& pFile) {

    if ( pFile.isEmpty() ) {
        return false;
    }

    QFileInfo lFileInfo(pFile);

    qDebug() << "Path : " << lFileInfo.dir().path();

    return lFileInfo.exists() && lFileInfo.isFile();

}


bool WalletHandler::walletDirectoryExists(const QString& pFile) {

    if ( pFile.isEmpty() ) {
        return false;
    }

    QFileInfo lFileInfo(pFile);

    return lFileInfo.dir().exists();

}



bool WalletHandler::createWallet(const QString& pFile, const QString& pPassword)
{

    if ( !walletDirectoryExists(pFile) ) {
        qWarning() << "Path doesn't exists : " << pFile;
        return false;
    }

    if ( walletFileExists(pFile) ) {
        qWarning() << "Wallet file " << pFile << "already exists";
        return false;
    }

    if ( pPassword.isEmpty() ) {
        qWarning() << "No password defined";
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


bool WalletHandler::tryWallet(const QString& pFile, const QString& pPassword) {

    if ( !walletFileExists(pFile) ) {
        qWarning() << "Wallet file " << pFile << "doesn't exists";
        return false;
    }

    if ( pPassword.isEmpty() ) {
        qWarning() << "No password defined";
        return false;
    }

    QProcess lTryWalletProcess;
    lTryWalletProcess.setProgram(mainProcess.program());

    QStringList lArguments;

    lArguments.append("--wallet=" + pFile);
    lArguments.append("--password="+ pPassword);
    lArguments.append("--command=getbalance"+ pPassword);
    lArguments.append("--exit-after-cmd=true");
    lTryWalletProcess.setArguments(lArguments);

    lTryWalletProcess.start();

    if ( !lTryWalletProcess.waitForFinished(2000) ) {
        qWarning() << "tryWallet failed : SubProcess doesn't responded";

        qWarning() << "Please ensure a wallet file exists at : " << pFile;
        qWarning() << "Please ensure that your executable is located at : " << lTryWalletProcess.program();

        return false;
    }

    int lExitCode = lTryWalletProcess.exitCode();
    qDebug() << "PROCESS finished with : " << QString::number(lExitCode);

    return lExitCode == 0;

}
