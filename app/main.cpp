
#include <QCoreApplication>
#include <QGuiApplication>

#include <QWindow>
#include <QIcon>

#include "MoneroGUI.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QString lCustomConfigFile;
    if (argc == 1) {
        lCustomConfigFile = QDir::homePath() + "/.bitmonero-qt/bitmonero-qt.conf";
    }
    else if (argc == 2) {
        lCustomConfigFile = QString::fromUtf8(argv[1]);
    }
    else {
        qCritical() << "Use config file path as first and only argument";
        return 1;
    }

    qDebug() << "Using config file : " << lCustomConfigFile;

    QIcon lIcon(":qml/res/MoneroWhite.ico");
    app.setWindowIcon(lIcon);

    MoneroGUI lGui(app, lCustomConfigFile);
    return lGui.start();
}

