include(../defaults.pri)

CONFIG += 64bit

CONFIG(32bit) {
    TARGET = bitmonero-qt-32bit
    QMAKE_CXXFLAGS += -m32
    LIBS +=
}
CONFIG(64bit) {
    TARGET = bitmonero-qt
}

TEMPLATE = app
QT += qml quick

CONFIG += console c++11

SOURCES += main.cpp \
    MoneroGUI.cpp

HEADERS += MoneroGUI.h

RESOURCES += qml.qrc

LIBS += -L../src -lbitmonero-qt-static

OTHER_FILES +=
