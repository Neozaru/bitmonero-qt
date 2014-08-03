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


    property bool iamVisible: transactionsTable.currentRow == 0

    RowLayout {
        id: controlsLayout

        Button {
            id: copyTxHashButton

            text: qsTr("Copy transaction Hash")
            enabled: transactionsTable.currentRow >= 0
            onClicked: {

                var hash = transactionsTable.model[transactionsTable.currentRow].id

                console.log(hash)
                /* TODO : Make a C++ interface for clipboard in order to remove this UGLY hack */
                stubCopyPasteTextEdit.text = hash;
                stubCopyPasteTextEdit.selectAll();
                stubCopyPasteTextEdit.copy();
                stubCopyPasteTextEdit.text = "";
            }
        }


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


    TabView {
        id: userTransactionsTabView

        anchors {
            top: controlsLayout.bottom
            topMargin: 5
            bottom: parent.bottom
            left: parent.left
            right: parent.right
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



    }


    Component {
        id: detailledTransfersTab

        RawTransfersTable {

            id: detailledTransfersTable

            model: wallet.transactions

        }
    }

}
