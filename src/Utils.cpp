#include "Utils.h"

Utils::Utils()
{
//    NOP
}


/* Returns first executable pFilenames found in pPaths */
const QStringList Utils::findExecutables(const QStringList& pPaths, const QStringList& pFilenames) {

    QStringList lFoundExecutables;

    for( const QString& lPath : pPaths ) {

        for ( const QString& lFilename : pFilenames ) {
            QFileInfo lFile(lPath + "/" + lFilename);

            if ( lFile.exists() && lFile.isFile() ) {
                lFoundExecutables.append(lFile.filePath());

            }

        }
    }

    return lFoundExecutables;

}
