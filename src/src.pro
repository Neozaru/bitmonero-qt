include(../defaults.pri)

QT += core network
CONFIG += qt staticlib c++11

TEMPLATE = lib
TARGET = bitmonero-qt-static


contains(CUSTOM_LIBMONEROWALLET, "yes") {
    CONFIG += libMoneroWallet
}
libMoneroWallet:DEFINES += LIBMONEROWALLET_ENABLED


SOURCES += \
    Models/WalletModel.cpp \
    Models/MoneroModel.cpp \
    Models/MinerModel.cpp \
    Models/AbstractModel.cpp \
    WalletSettings.cpp \
    DaemonHandler.cpp \
    Utils.cpp \
    Models/TransactionModel.cpp \
    Models/InfoWalletModel.cpp \
    Models/WalletHandlerModel.cpp \
    Models/ApplicationModel.cpp

libMoneroWallet:SOURCES += MoneroWalletWrappers/WrapperWallet.cpp
libMoneroWallet:SOURCES += MoneroWalletWrappers/WrapperWalletHandler.cpp

SOURCES += \
    Builders/WalletInterfaceBuilder.cpp \
    Builders/WalletHandlerInterfaceBuilder.cpp \
    Builders/MinerInterfaceBuilder.cpp

SOURCES += \
    RPC/JsonRPCCommunicator.cpp \
    RPC/JsonRPCRequest.cpp \
    RPC/RPCWallet.cpp \
    RPC/RPCMonero.cpp \
    RPC/RPCMiner.cpp \
    RPC/WalletHandlerProcess.cpp


HEADERS += \
    Models/WalletModel.h \
    Models/MoneroModel.h \
    Interfaces/MoneroInterface.h \
    Interfaces/WalletInterface.h \
    Interfaces/MinerInterface.h \
    Models/MinerModel.h \
    Models/AbstractModel.h \
    WalletSettings.h \
    DaemonHandler.h \
    Utils.h \
    Models/TransactionModel.h \
    Models/InfoWalletModel.h \
    Interfaces/WalletHandlerInterface.h \
    Models/WalletHandlerModel.h \
    Models/ApplicationModel.h \
    Interfaces/MetaInterface.h


libMoneroWallet:HEADERS += MoneroWalletWrappers/WrapperWallet.h
libMoneroWallet:HEADERS += MoneroWalletWrappers/WrapperWalletHandler.h

HEADERS += \
    Builders/WalletInterfaceBuilder.h \
    Builders/WalletHandlerInterfaceBuilder.h \
    Builders/MinerInterfaceBuilder.h

HEADERS += \
    RPC/JsonRPCCommunicator.h \
    RPC/JsonRPCRequest.h \
    RPC/RPCWallet.h \
    RPC/RPCMonero.h \
    RPC/RPCMiner.h \
    RPC/WalletHandlerProcess.h
