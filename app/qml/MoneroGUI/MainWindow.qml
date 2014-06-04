import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "Common"

ApplicationWindow {
    id: mainWindow

    visible: false
    title: "Monero Wallet"

    width: 750
    height: 480


//    Action {
//        id: openAction
//        text: "&Open"
//        shortcut: StandardKey.Open
////        iconSource: "images/document-open.png"
//        onTriggered: fileDialog.open()
//        tooltip: "Open an image"
//    }

    AboutDialog { id: aboutDialog }

    Action {
        id: aboutAction
        text: "About"
        onTriggered: aboutDialog.open()
    }



    Menu {
        id: editmenu
//        MenuItem { action: cutAction }
        MenuSeparator {}
        Menu {
            title: "Text &Format"
//            MenuItem { action: a1 }
        }

    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
//            MenuItem { action: openAction }
            MenuItem {
                text: "Close"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
//        Menu {
//            title: "&Edit"
////            MenuItem { action: cutAction }
////            MenuItem { action: copyAction }
////            MenuItem { action: pasteAction }
//            MenuSeparator { }
//            MenuItem {
//                text: "Do Nothing"
//                shortcut: "Ctrl+E,Shift+Ctrl+X"
//                enabled: false
//            }
//            MenuItem {
//                text: "Not Even There"
//                shortcut: "Ctrl+E,Shift+Ctrl+Y"
//                visible: false
//            }
//            Menu {
//                title: "Me Neither"
//                visible: false
//            }
//        }
        Menu {
            title: "&Help"
            MenuItem { action: aboutAction }
        }
    }
    toolBar: ToolBar {
        id: toolbar
//        height: 30
        RowLayout {
            id: toolbarLayout
            spacing: 0
            width: parent.width
            ToolButton {
//                iconSource: "images/window-new.png"
                onClicked: window1.visible = !window1.visible
                Accessible.name: "New window"
                tooltip: "Toggle visibility of the second window"
            }
//            ToolButton { action: openAction }
            ToolButton {
//                iconSource: "images/document-save-as.png"
                tooltip: "(Pretend to) Save as..."
            }
            Item { Layout.fillWidth: true }
            CheckBox {
                id: advancedInterfaceCheckbox
                text: "Advanced Interface"
                checked: false
                enabled: false
            }
        }
    }



    SystemPalette {id: syspal}
    color: syspal.window
    ListModel {
        id: choices
        ListElement { text: "Banana" }
        ListElement { text: "Orange" }
        ListElement { text: "Apple" }
        ListElement { text: "Coconut" }
    }


    statusBar: StatusBar {

        RowLayout {
            anchors.fill: parent

            RowLayout {
                anchors.left: parent.left

                Label {
                    text: "Balance : "
                }

                Balance {
                    balance: wallet.balance
                }
            }

            RowLayout {
                id: syncingStatusLayout

                visible: true
                anchors.centerIn: parent

                property int syncing_progress: monero.target_blockchain_height > 0 ? monero.blockchain_height / monero.target_blockchain_height * 100 : 0

                ProgressBar {
                    id: syncStatusProgressBar

                    anchors.centerIn: parent

                    minimumValue: 0
                    maximumValue: 100
                    value: syncingStatusLayout.syncing_progress
                    width: 50

                }

                Label {

                    anchors.horizontalCenter: syncStatusProgressBar.horizontalCenter
                    text: "Syncing : " + (syncingStatusLayout.syncing_progress > 0 ? syncingStatusLayout.syncing_progress + "%" : "Unknown")
                }

                Label {
                    visible: false
                    text: "(" + monero.blockchain_height + " blks)";
                }


            }

            RowLayout {
                anchors.right: parent.right

                Label {
                    text: "Mining : "
                }


                HashRate {
                    visible: miner.enabled
                    hashrate: miner.hashrate
                    color: "green"
                }

                Label {
                    visible: miner.enabled
                    text: "(" + (miner.nbThreads) + "T)"
                }

                Label {
                    visible: !miner.enabled
                    text: "Disabled"
                }
            }

        }

    }
    TabView {
        id:frame

        property string unavailableWalletErrorMessage: wallet.ready ? "" : "Error : Your Wallet is unreachable. Please check 'simplewallet' is running and bound to correct RPC port"
        property string unavailableMinerErrorMessage: miner.ready ? "" : "Error : Your Miner is unreachable. Please check 'bitmonerod' is running and bound to correct RPC port"

        anchors.fill: parent

        anchors.margins: Qt.platform.os === "osx" ? 12 : 2

        Tab {
            id: controlPage


            title: "Overview"
            Overview {
                enabled: wallet.ready
                errorMessage: frame.unavailableWalletErrorMessage

            }

        }
        Tab {
            title: "Transfer"
            Transfer {
                enabled: wallet.ready
                errorMessage: frame.unavailableWalletErrorMessage
            }

        }

        Tab {
            title: "History (Not impl)"
            enabled: false
            History {}
        }


        Tab {
            id: miningPage
            title: "Mining"
            Mining {
                enabled: miner.ready
                errorMessage: frame.unavailableMinerErrorMessage
            }
        }

//        Tab {
//            title: "Debug"
//        }

    }


}
