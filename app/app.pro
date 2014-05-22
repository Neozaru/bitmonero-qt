include(../defaults.pri)

TEMPLATE = app
QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

LIBS += -L../src -lbitmonero-qt-lib
