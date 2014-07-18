import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0


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

        CheckBox {
            id: advancedViewCheckbox

            visible: mainWindow.advancedInterface
            text: qsTr("Detailled transfers")
        }

    }

    TableView {

        id: transactionsTable

        anchors {
            top: controlsLayout.bottom
            topMargin: 5
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        alternatingRowColors: true
        sortIndicatorColumn: 1
        sortIndicatorOrder: Qt.DescendingOrder
        sortIndicatorVisible: true


        model: (mainWindow.advancedInterface && advancedViewCheckbox.checked) ? wallet.transactions : wallet.aggregated_transactions


        TableViewColumn {
            role: "block_height" ;
            title: qsTr("Block", "(tx list)");
            width: 80;
        }

        TableViewColumn {
            role: "date" ;
            title: qsTr("Date", "(tx list)");
            width: 120;


            delegate: Text {
                text: Qt.formatDateTime(styleData.value)
            }
        }

//        TableViewColumn {

//            role: "type";
//            title: qsTr("Type", "(tx list)");
//            width: 45

//            delegate: Text {
//                text: styleData.value ? styleData.value : " ?"
//            }
//        }

        TableViewColumn{

            role: "amount";
            title: qsTr("Amount", "(tx list)");
            width: 120;

            delegate: Text {
                text: (Math.pow(10,-12)*parseInt(styleData.value)).toFixed(12)
            }
        }
        TableViewColumn{
            role: "spendable" ;
            title: qsTr("Spendable","(tx list)") ;
            width: 40;

            delegate: Text {
                text: styleData.value ? qsTr("yes") : qsTr("no")
            }
        }

        TableViewColumn {
            role: "id" ;
            title: qsTr("Tx hash", "(tx list)");
            width: 750;
        }


    }


}
