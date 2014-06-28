
#include <QCoreApplication>
#include <QGuiApplication>

#include <QWindow>
#include <QIcon>

#include "MoneroGUI.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QIcon lIcon(":qml/res/MoneroWhite.ico");
    app.setWindowIcon(lIcon);

    MoneroGUI lGui(app);
    return lGui.start();
}

