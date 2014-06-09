#include "DaemonHandler.h"

#include <QDebug>

#include "Utils.h"
#include <QCoreApplication>

DaemonHandler::DaemonHandler(const WalletSettings& pSettings)
{

    /* Try to get bitmonerod program from configuration */
    QString lDaemonProgram = pSettings.getDaemonProgram();

    /* If no program defined, scan some folders */
    if (lDaemonProgram.isEmpty()) {

        const QString& lAppPath = QCoreApplication::applicationDirPath();
        QStringList lSearchPaths;
        lSearchPaths.append( QDir::currentPath() );
        lSearchPaths.append( QDir::homePath() + "/.bitmonero-qt/");
        lSearchPaths.append( lAppPath );
        lSearchPaths.append( lAppPath + "/bitmonero/");

        lSearchPaths.append( "/usr/bin" );
        lSearchPaths.append( "/usr/local/bin" );

        QStringList lMoneroSearchFilenames;
        lMoneroSearchFilenames.append("bitmonerod");
        lMoneroSearchFilenames.append("bitmonerod.exe");



        QStringList lFoundMoneroExecutables = Utils::findExecutables(lSearchPaths, lMoneroSearchFilenames, true);
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

    if (terminateDaemon()) {
        qWarning() << "Ending DAEMON process...";
        if (!main_process.waitForFinished(10000)) {

            qWarning() << "DAEMON took too long to close. KILL";
            main_process.kill();
        }


    }

}

bool DaemonHandler::execDaemon()
{

    qDebug() << "STARTING DAEMON...";

    main_process.start();

    /* Should not finish */
    bool res = !main_process.waitForFinished(1000);
    qDebug() << "DAEMON STATUS : " << main_process.state();

    return res;

}

bool DaemonHandler::terminateDaemon()
{
    if (main_process.state() == QProcess::ProcessState::NotRunning) {
        return false;
    }

    main_process.terminate();
    return true;
}
