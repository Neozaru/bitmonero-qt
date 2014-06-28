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

contains(CUSTOM_LIBMONEROWALLET, "yes") {
    CONFIG += libMoneroWallet
}

TEMPLATE = app
QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    MoneroGUI.cpp

lupdate_only {
SOURCES = qml/*.qml \
          qml/Utils/*.qml \
          qml/Wizard/*.qml \
          qml/MoneroGUI/*.qml \
          qml/MoneroGUI/Common/*.qml
}

HEADERS += MoneroGUI.h

RESOURCES += qml.qrc

CONFIG(unix) {

    CONFIG(libMoneroWallet) {
        CONFIG(release):LIBS += -L$$PWD/../src -lbitmonero-qt-static -lmonerowallet
        CONFIG(debug):LIBS += -L$$PWD/../src -lbitmonero-qt-static -lmonerowallet
    }
    else {
        CONFIG(release):LIBS += -L$$PWD/../src -lbitmonero-qt-static
        CONFIG(debug):LIBS += -L$$PWD/../src -lbitmonero-qt-static
    }
}

CONFIG(win32) {
    CONFIG(release):LIBS += -L$$PWD\..\src\release -lbitmonero-qt-static
    CONFIG(debug):LIBS += -L$$PWD\..\src\debug -lbitmonero-qt-static
}

OTHER_FILES +=
