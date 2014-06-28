TEMPLATE = subdirs

SUBDIRS = \
    src \
    app \
#    tests

RC_ICONS = app/qml/res/MoneroWhite.ico

app.depends = src
#RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

CONFIG += LibMoneroWallet
# Default rules for deployment.
include(defaults.pri)
include(deployment.pri)


contains(CUSTOM_LIBMONEROWALLET, "yes") {
    message("----------------------------- Root with LibMoneroWallet")
    CONFIG += libMoneroWallet
}
