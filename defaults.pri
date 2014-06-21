#CONFIG += libMoneroWallet

INCLUDEPATH += $$PWD/src
SRC_DIR = $$PWD


CONFIG(libMoneroWallet) {
    CUSTOM_LIBMONEROWALLET = "yes"
}
else {
    CUSTOM_LIBMONEROWALLET = "no"
}
