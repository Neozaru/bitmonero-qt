#include "Utils.h"

#include <QCoreApplication>
#include <QDir>

Utils::Utils()
{
//    NOP
}


/* Returns first executable pFilenames found in pPaths */
const QStringList Utils::findExecutables(const QStringList& pPaths, const QStringList& pFilenames, bool pFindOne) {

    QStringList lFoundExecutables;

    for( const QString& lPath : pPaths ) {

        for ( const QString& lFilename : pFilenames ) {
            QFileInfo lFile(lPath + "/" + lFilename);

            if ( lFile.exists() && lFile.isFile() ) {
                lFoundExecutables.append(lFile.filePath());
                if (pFindOne) {
                    return lFoundExecutables;
                }
            }

        }
    }

    return lFoundExecutables;

}


const QStringList Utils::getStandardSearchPaths() {

    const QString& lAppPath = QCoreApplication::applicationDirPath();

    QStringList lSearchPaths;
    lSearchPaths.append( QDir::currentPath() );
    lSearchPaths.append( QDir::homePath() + "/.bitmonero-qt/");
    lSearchPaths.append( lAppPath );
    lSearchPaths.append( lAppPath + "/bitmonero/");

    lSearchPaths.append( "/usr/bin" );
    lSearchPaths.append( "/usr/local/bin" );


    return lSearchPaths;
}
