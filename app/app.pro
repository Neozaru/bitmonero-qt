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

CONFIG += c++11

SOURCES += main.cpp \
    MoneroGUI.cpp

HEADERS += MoneroGUI.h

RESOURCES += qml.qrc

unix:CONFIG(release):LIBS += -L$$PWD/../src -lbitmonero-qt-static
unix:CONFIG(debug):LIBS += -L$$PWD/../src -lbitmonero-qt-static

win32:CONFIG(release):LIBS += -l$$PWD\..\src\release -lbitmonero-qt-static
win32:CONFIG(debug):LIBS += -l$$PWD\..\src\debug -lbitmonero-qt-static

OTHER_FILES +=
