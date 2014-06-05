#include "DaemonHandler.h"

#include <QDebug>

#include "Utils.h"

//DaemonHandler::DaemonHandler(const QString& pProgramPath)
//{
//    main_process.setProgram(pProgramPath);
//}

DaemonHandler::DaemonHandler(const WalletSettings& pSettings)
{

    /* Try to get bitmonerod program from configuration */
    QString lDaemonProgram = pSettings.getDaemonProgram();

    /* If no program defined, scan some folders */
    if (lDaemonProgram.isEmpty()) {

        QStringList lSearchPaths;
        lSearchPaths.append( QDir::currentPath() );
        lSearchPaths.append( QDir::currentPath() + "/bitmonero/");
        lSearchPaths.append( "/usr/bin" );
        lSearchPaths.append( "/usr/local/bin" );

        QStringList lMoneroSearchFilenames;
        lMoneroSearchFilenames.append("bitmonerod");
        lMoneroSearchFilenames.append("bitmonerod.exe");



        QStringList lFoundMoneroExecutables = Utils::findExecutables(lSearchPaths, lMoneroSearchFilenames);
        qDebug() << "Found " << lFoundMoneroExecutables.size() << " monero daemon executables : ";
        for( const QString& lMoneroExec : lFoundMoneroExecutables ) {
            qDebug() << "- " << lMoneroExec;
        }

        if (!lFoundMoneroExecutables.empty()) {
            lDaemonProgram = lFoundMoneroExecutables.first();
        }

    }

    if (!lDaemonProgram.isEmpty()) {

        main_process.setProgram(lDaemonProgram);
    }



}

DaemonHandler::~DaemonHandler() {

    if (killDaemon()) {
        qWarning() << "Ending DAEMON process...";
        main_process.waitForFinished(10000);
    }

}

bool DaemonHandler::execDaemon()
{

    qDebug() << "STARTING DAEMON...";

    main_process.start();

    /* Should not finish */
    bool res = !main_process.waitForFinished(5000);
    qDebug() << "DAEMON STATUS : " << main_process.state();

    return res;

}

bool DaemonHandler::killDaemon()
{
    if (main_process.state() == QProcess::ProcessState::NotRunning) {
        return false;
    }

    main_process.terminate();
    return true;
}
