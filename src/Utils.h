#ifndef UTILS_H
#define UTILS_H

#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QList>
#include <QObject>

class InfoWalletModel;

class Utils
{
public:
    Utils();

    /* Returns first executable pFilenames found in pPaths */
    static const QStringList findExecutables(const QStringList& pPaths, const QStringList& pFilenames, bool pFindOne = false);

    static const QStringList getStandardSearchPaths();

    static const QStringList findWalletsKeysFiles(const QUrl &pUrl, const QString& pFileSuffix = ".keys");

    static const QString extractWalletAddress(const QString& pWalletPath);

    static const QList<QObject*> fileListToInfoWalletModelList(const QStringList& pWalletFilesList, const QUrl& pFolderUrl);

};

#endif // UTILS_H
