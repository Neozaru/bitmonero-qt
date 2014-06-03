include(../defaults.pri)

TARGET= bitmonero-qt
TEMPLATE = app
QT += qml quick

CONFIG += console c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

LIBS += -L../src -lbitmonero-qt-static

OTHER_FILES +=
