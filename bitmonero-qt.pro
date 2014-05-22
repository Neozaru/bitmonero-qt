TEMPLATE = subdirs

SUBDIRS = \
    src \
    app \
#    tests

#QT += core network qml quick

CONFIG += ordered


app.depends = src
#tests.depends = src

#RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

