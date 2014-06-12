#include "WalletHandler.h"

#include <QStringList>
#include <QDebug>

#include <QDir>

#include "Utils.h"

WalletHandler::WalletHandler(const WalletSettings& pWalletSettings)
    : open(false), default_wallet_location(QDir::homePath() + "/.bitmonero/")
{

    QString lWalletProgram = pWalletSettings.getWalletProgram();

    if (lWalletProgram.isEmpty()) {

        const QStringList& lSearchPaths = Utils::getStandardSearchPaths();

        QStringList lWalletSearchFilenames;
        lWalletSearchFilenames.append("simplewallet");
        lWalletSearchFilenames.append("simplewallet.exe");


        QStringList lFoundWalletExecutables = Utils::findExecutables(lSearchPaths, lWalletSearchFilenames, true);


        qDebug() << "Found " << lFoundWalletExecutables.size() << " wallet executables : ";
        for( const QString& lWalletExec : lFoundWalletExecutables ) {
            qDebug() << "- " << lWalletExec;
        }

        if (!lFoundWalletExecutables.empty()) {
            lWalletProgram = lFoundWalletExecutables.first();
        }

    }

    if (!lWalletProgram.isEmpty()) {

        main_process.setProgram(lWalletProgram);
    }

}

WalletHandler::~WalletHandler() {

    try {

        if(closeWallet()) {

            qWarning() << "Ending Wallet process...";
            if(!main_process.waitForFinished(5000)){
                main_process.kill();
                main_process.waitForFinished(1000);
            }

        }
    }
    catch(std::exception e) {
        qDebug() << "Exception during Wallet closing : " << e.what();
    }

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


    /* TODO : Remove */
    main_process.setProcessChannelMode(QProcess::MergedChannels);
    main_process.start();

    /* Wait 3 seconds to check if program terminates (daemon not running, wrongp password ... */
    bool lFinished = main_process.waitForFinished(3000);
    qDebug() << "Wallet State : " << main_process.state();

    if (lFinished) {
        qWarning() << "Wallet process finished too early with code " << main_process.exitCode() << ", " << main_process.exitStatus();
        return false;
    }

    open = true;

    qDebug() << "Wallet process started on " + pBindIP + ":" + QString::number(pBindPort) + " (" + main_process.program() + ")";

    return true;

}

bool WalletHandler::closeWallet() {

    open = false;

    if ( main_process.state() == QProcess::Running ) {
        main_process.terminate();
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

    /* Attempts to create directory if doesn't exists */
    return lFileInfo.dir().exists() || QDir::root().mkpath(lFileInfo.absolutePath());

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


void WalletHandler::tryWalletResponse(int pCode, QProcess::ExitStatus pExitStatus) {

    qWarning() << "=======================";
    qWarning() << "tryWalletAsync returned status : " << pCode << ". " << pExitStatus << ".";
    qWarning() << "=======================";
    emit tryWalletResult(pCode == 0);

}


bool WalletHandler::tryWalletAsync(const QString& pFile, const QString& pPassword) {

    QProcess* lTryWalletProcess = execTryWallet(pFile,pPassword);
    if (!lTryWalletProcess) {
        emit tryWalletResult(false);
        return false;
    }

    QObject::connect(lTryWalletProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(tryWalletResponse(int,QProcess::ExitStatus)));
    return true;

}

QProcess* WalletHandler::execTryWallet(const QString& pFile, const QString& pPassword) {

    qDebug() << "Trying wallet : " << pFile;

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

    /* TODO : Remove */
    lTryWalletProcess->setStandardOutputFile("/tmp/test.txt");

    lTryWalletProcess->start();

    return lTryWalletProcess;

}
