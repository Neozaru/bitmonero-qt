
#include <QCoreApplication>
#include <QGuiApplication>


#include "MoneroGUI.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    MoneroGUI lGui(app);
    return lGui.start();
}

