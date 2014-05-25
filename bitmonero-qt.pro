TEMPLATE = subdirs

SUBDIRS = \
    src \
    app \
#    tests


CONFIG += ordered

app.depends = src

#RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

