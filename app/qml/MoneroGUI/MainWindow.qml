import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "Common"

ApplicationWindow {
    id: mainWindow

    visible: false
    title: "Monero Wallet"


    onClosing: {
        Qt.quit();
    }


    AboutDialog { id: aboutDialog }

    Action {
        id: aboutAction
        text: "About"
        onTriggered: aboutDialog.open()
    }



    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem {
                text: "Close"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: "&Help"
            MenuItem { action: aboutAction }
        }
    }
    toolBar: ToolBar {
        id: toolbar

        height: 50
        RowLayout {
            id: toolbarLayout
            anchors.verticalCenter: parent.verticalCenter

            spacing: 0
            width: parent.width
//            ToolButton {
////                iconSource: "images/window-new.png"
//                onClicked: window1.visible = !window1.visible
//                Accessible.name: "New window"
//                tooltip: "Toggle visibility of the second window"
//            }


            Item { Layout.fillWidth: true }
            CheckBox {
                id: advancedInterfaceCheckbox
//                anchors.verticalCenter: parent.verticalCenter

                text: "Advanced Interface"

                checked: false
                enabled: false
            }
        }
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

                property int syncing_progress: monero.target_blockchain_height > 0 ? (monero.blockchain_height / monero.target_blockchain_height) * 100 : -1

                property int syncing_progress_user: syncing_progress < 0 ? -1 : (syncing_progress > 100 ? 100 : syncing_progress)
                ProgressBar {
                    id: syncStatusProgressBar

                    anchors.centerIn: parent

                    minimumValue: 0
                    maximumValue: 100
                    value: syncingStatusLayout.syncing_progress_user == -1 ? 0 : syncingStatusLayout.syncing_progress_user
                    width: 50

                }

                Label {

                    anchors.horizontalCenter: syncStatusProgressBar.horizontalCenter

                    text: "Syncing : " + (syncingStatusLayout.syncing_progress_user == -1 ? "unknown" : (syncingStatusLayout.syncing_progress_user) + "%")
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

        property string unavailableWalletErrorMessage: wallet.ready ? "" : "Your Wallet is unreachable. Wait a few seconds or restart Monero Wallet"
        property string unavailableMinerErrorMessage: miner.ready ? "" : "Error : Your Miner is unreachable. Wait for sync or restart Monero Wallet"

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
            title: "History (beta)"
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

        /* OMG I'm a so bad designer. Tried some colors ... :/ */
        style: TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
//                color: styleData.selected ? "steelblue" :"lightsteelblue"
                    color: styleData.selected ? "#FAFAFA" :"#EBEBEB"
//                color: "#E3E4FA"
//                border.color:  "steelblue"
//                border.color: "#EBEBEB"
                    border.color: "#FAFAFA"

                implicitWidth: Math.max(text.width + 4, 80)*2
                implicitHeight: 50
                radius: 2
                Text {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
//                    color: styleData.selected ? "white" : "black"
                    color: "black"
                    font.pixelSize: 18
                }
            }
            frame: Rectangle {

                /*color: "steelblue"*/ /* color: "#EDEEFA" */
                color: "#FAFAFA"
//                color: "#F5F5FC";
//                color: "#FCFDFF"
//                color: "#FCFCFC"
            }
        }

    }


}
