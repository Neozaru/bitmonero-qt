import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0

import "Common"

ColumnLayout {

    anchors.fill: parent;
    anchors.margins: 10;

    TextEdit {
        id: stubCopyPasteTextEdit
        visible: false;
        enabled: false;

    }


    Connections {
        target: mainWindow
        onAdvancedInterfaceChanged: {
            if (mainWindow.advancedInterface) {
                userTransactionsTabView.addTab("Details", detailledTransfersTab);
            }
            else if (userTransactionsTabView.count >= 3) {
                userTransactionsTabView.removeTab(2);

            }
        }
    }

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 2

        Button {
            id: copyTxHashButton

            text: qsTr("Copy transaction Hash")
//            enabled: userTransactionsTabView.getCurrentTable().currentRow >= 0
            enabled: false

            onClicked: {

                var table = userTransactionsTabView.getCurrentTable();
                var hash = table.model[table.currentRow].id

                console.log(hash)
                /* TODO : Make a C++ interface for clipboard in order to remove this UGLY hack */
                stubCopyPasteTextEdit.text = hash;
                stubCopyPasteTextEdit.selectAll();
                stubCopyPasteTextEdit.copy();
                stubCopyPasteTextEdit.text = "";
            }
        }



        TabView {
            id: userTransactionsTabView

            anchors {
                top: copyTxHashButton.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom

                margins: Qt.platform.os === "osx" ? 12 : 2
            }

            Tab {
                id: userIncomingTransfersTab

                title: qsTr("Received", "Transaction history tab title")

                UserTransfersTable {

                    id: userIncomingTransfersTable

                    model: wallet.aggregated_incoming_transactions

                }


            }

            Tab {
                id: userOutgoingTransfersTab

                title: qsTr("Sent", "Transaction history tab title2")

                UserTransfersTable {

                    id: userOutgoingTransfersTable

                    model: wallet.aggregated_outgoing_transactions

                }
            }


            function getCurrentTable() {
                var index = currentIndex
                console.log("Index:" + index)

                switch (index) {
                    case 0:
                        return userIncomingTransfersTab.component
                    case 1:
                        return userOutgoingTransfersTab.component
                    case 2:
                        return detailledTransfersTable
                }
            }


        }

    }

    Component {
        id: detailledTransfersTab

        RawTransfersTable {

            id: detailledTransfersTable
            model: wallet.transactions

        }


    }

}
