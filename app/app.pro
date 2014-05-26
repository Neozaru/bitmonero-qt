include(../defaults.pri)

TARGET= bitmonero-qt
TEMPLATE = app
QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

LIBS += -L../src -lbitmonero-qt-static

OTHER_FILES +=
