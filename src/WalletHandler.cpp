#include "WalletHandler.h"

#include <iostream>

#include <QStringList>
#include <QDebug>

#include <QIODevice>


WalletHandler::WalletHandler(const QString& pProgramPath)
    : open(false)
{
    main_process.setProgram(pProgramPath);

}

bool WalletHandler::openWalletAsync(const QString& pWalletFile, const QString& pWalletPassword, const QString& pBindIP, int pBindPort) {

    if ( main_process.state() == QProcess::Running ) {
        qWarning() << "Wallet is already open. Please close your wallet before opening another one";
        return false;
    }


    QStringList lArguments;

    lArguments.append("--wallet=" + pWalletFile);
    lArguments.append("--pass=" + pWalletPassword);
    lArguments.append("--rpc-bind-ip=" + pBindIP);
    lArguments.append("--rpc-bind-port=" + QString::number(pBindPort));

    main_process.setArguments(lArguments);

    main_process.start();

    /* Wait 3 seconds to check if program terminates (daemon not running, wrongp password ... */
    bool lFinished = main_process.waitForFinished(3000);
    qDebug() << "Finished ? " << lFinished;
    qDebug() << "State : " << main_process.state();

    if (lFinished) {
        return false;
    }

    open = true;

    qDebug() << "Wallet process started on " + pBindIP + ":" + QString::number(pBindPort) + " (" + main_process.program() + ")";


    return true;

}

bool WalletHandler::closeWallet() {

    open = false;

    if ( main_process.state() == QProcess::Running ) {
        main_process.kill();
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
    lCreateWalletProcess.setProgram(main_process.program());

    QStringList lArguments;

    lArguments.append("--generate-new-wallet=" + pFile);
    lArguments.append("--password="+ pPassword);
    /* Hack for forcing program to exit after generation */
    lArguments.append("--command=address");
    /**/
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

bool WalletHandler::tryWalletProgram()
{
    QProcess lTryWalletProgramProcess;
    lTryWalletProgramProcess.setProgram(main_process.program());

    QStringList lArguments;
    lArguments.append("--help");

    lTryWalletProgramProcess.setArguments(lArguments);

    lTryWalletProgramProcess.start();


    return lTryWalletProgramProcess.waitForFinished(2000);

}

void WalletHandler::tryWalletResponse(int pCode) {

    emit tryWalletResult(pCode == 0);

}

bool WalletHandler::tryWallet(const QString& pFile, const QString& pPassword)
{

    QProcess* lTryWalletProcess = execTryWallet(pFile,pPassword);
    if (!lTryWalletProcess) {
        return false;
    }

    std::cout << "Wait" << std::endl;
    if ( !lTryWalletProcess->waitForFinished(60000) ) {
        std::cout << "SUBP" << std::endl;
        qWarning() << "tryWallet failed : SubProcess doesn't responded";

        qWarning() << "Please ensure a wallet file exists at : " << pFile;

        return false;
    }
    std::cout << "Responded !" << std::endl;


    int lExitCode = lTryWalletProcess->exitCode();
    qDebug() << "PROCESS finished with : " << QString::number(lExitCode);

    return lExitCode == 0;

}

bool WalletHandler::tryWalletAsync(const QString& pFile, const QString& pPassword) {

    QProcess* lTryWalletProcess = execTryWallet(pFile,pPassword);
    if (!lTryWalletProcess) {
        return false;
    }

    QObject::connect(lTryWalletProcess, SIGNAL(finished(int)), this, SLOT(tryWalletResponse(int)));
    return true;

}

QProcess* WalletHandler::execTryWallet(const QString& pFile, const QString& pPassword) {

        std::cout << "TRY wallet : " << pFile.toStdString() << std::endl;

    if ( !tryWalletProgram() ) {
        qWarning() << "Wallet program not responding";
        qWarning() << "Please ensure that your executable is located at : " << main_process.program();
        return NULL;
    }

    if ( !walletFileExists(pFile) ) {
        std::cout << "Dindonneau" << std::endl;
        qWarning() << "Wallet file " << pFile << "doesn't exists";
        return NULL;
    }

    if ( pPassword.isEmpty() ) {
        qWarning() << "No password defined";
        return NULL;
    }


    QProcess* lTryWalletProcess = new QProcess();
    lTryWalletProcess->setProgram(main_process.program());

    QStringList lArguments;

    lArguments.append("--wallet=" + pFile);
    lArguments.append("--password="+ pPassword);
    lArguments.append("--command=getbalance");
    lTryWalletProcess->setArguments(lArguments);

    lTryWalletProcess->start();

    return lTryWalletProcess;

}
