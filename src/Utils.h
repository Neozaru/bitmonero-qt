#ifndef UTILS_H
#define UTILS_H

#include <QStringList>
#include <QFileInfo>

class Utils
{
public:
    Utils();


    /* Returns first executable pFilenames found in pPaths */
    static const QStringList findExecutables(const QStringList& pPaths, const QStringList& pFilenames) {

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
};

#endif // UTILS_H
