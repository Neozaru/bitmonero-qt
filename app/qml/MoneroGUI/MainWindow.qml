import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "Common"

ApplicationWindow {
    id: mainWindow

    visible: false
    title: qsTr("Monero Wallet", "window title")

    property bool advancedInterface: false


    onClosing: {
        console.log("CLOSING")
        application.requestApplicationQuit()
    }


    AboutDialog { id: aboutDialog }

    Action {
        id: aboutAction
        text: "About"
        onTriggered: aboutDialog.open()
    }

    SettingsWindow {
        id: settingsWindow
    }


    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem {
                text: "Start Wizard"
//                shortcut: StandardKey.c
                onTriggered: application.requestLaunchWizard()
            }

            MenuItem {
                text: "Close"
                shortcut: StandardKey.Quit
                onTriggered: application.requestApplicationQuit()
            }
        }

        Menu {
            title: "&Edit"
            MenuItem {
                text: "Preferencees"
//                shortcut: StandardKey.c
                onTriggered: settingsWindow.show()
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

                checked: mainWindow.advancedInterface
                onCheckedChanged: {
                    mainWindow.advancedInterface = checked;
                }

            }
        }
    }

    statusBar: StatusBar {

        RowLayout {
            anchors.fill: parent

            RowLayout {
                anchors.left: parent.left

                Label {
                    text: qsTr("Balance") + " : "
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

                    text: qsTr("Syncing : %1").arg(syncingStatusLayout.syncing_progress_user == -1 ? qsTr("unknown","syncing status unknown") : (syncingStatusLayout.syncing_progress_user) + "%")
                }

                Label {
                    visible: false
                    text: "(" + qsTr("%L1 blks").arg(monero.blockchain_height) + ")";
                }


            }

            RowLayout {
                anchors.right: parent.right

                Label {
                    text: qsTr("Mining", "mining status") + " : "
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

        property string unavailableWalletErrorMessage: wallet.ready ? "" : qsTr("Your Wallet is unreachable. Wait a few seconds or restart Monero Wallet")
        property string unavailableMinerErrorMessage: miner.ready ? "" : qsTr("Your Miner is unreachable. Wait for sync or restart Monero Wallet")

        anchors {
            fill: parent
            margins: Qt.platform.os === "osx" ? 12 : 2
        }

        Tab {
            id: controlPage

            title: qsTr("Overview", "overview tab")
            Overview {
                enabled: wallet.ready
                errorMessage: frame.unavailableWalletErrorMessage

            }

        }
        Tab {
            title: qsTr("Transfer", "transfer tab")
            Transfer {
                enabled: wallet.ready
                errorMessage: frame.unavailableWalletErrorMessage
            }

        }

        Tab {
            title: qsTr("History", "history tab") + " (beta)"
            History {}
        }


        Tab {
            id: miningPage
            title: qsTr("Mining", "mining tab")
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
//                color: styleData.selected ? "white" :"#EBEBEB"

//                color: "#E3E4FA"

//                border.color:  "steelblue"
//                border.color: "#EBEBEB"
                  border.color: "#FAFAFA"
//                border.color: "white"

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
//                color: "white"
//                color: "#F5F5FC";
//                color: "#FCFDFF"
//                color: "#FCFCFC"
            }
        }

    }


}
