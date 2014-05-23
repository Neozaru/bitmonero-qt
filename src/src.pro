include(../defaults.pri)

QT += core network
CONFIG += qt

TEMPLATE = lib
TARGET = bitmonero-qt-lib

SOURCES += \
    Models/WalletModel.cpp \
    Models/MoneroModel.cpp \
    Models/TransactionsListModel.cpp

SOURCES += \
    RPC/JsonRPCCommunicator.cpp \
    RPC/JsonRPCRequest.cpp \
    RPC/RPCWallet.cpp \
    RPC/RPCMonero.cpp


HEADERS += \
    Models/WalletModel.h \
    Models/MoneroModel.h \
    Interfaces/MoneroInterface.h \
    Interfaces/WalletInterface.h \
    Models/TransactionsListModel.h

HEADERS += \
    RPC/JsonRPCCommunicator.h \
    RPC/JsonRPCRequest.h \
    RPC/RPCWallet.h \
    RPC/RPCMonero.h
