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

            text: "Copy Tx Hash"
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
            text: "Detailled transfers"
        }

    }


    TableView {

        id: transactionsTable

        anchors.top: controlsLayout.bottom
        anchors.topMargin: 5

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        alternatingRowColors: true


        model: (mainWindow.advancedInterface && advancedViewCheckbox.checked) ? wallet.transactions : wallet.aggregated_transactions
        TableViewColumn {

            role: "type";
            title: "Type";
            width: 45

            delegate: Text {
                text: styleData.value ? styleData.value : " ?"
            }
        }

        TableViewColumn{

            role: "amount" ;
            title: "Amount" ;
            width: 120;

            delegate: Text {
                text: (Math.pow(10,-12)*parseInt(styleData.value)).toFixed(12)
            }
        }
        TableViewColumn{
            role: "spendable" ;
            title: "Spendable" ;
            width: 40;

            delegate: Text {
                text: styleData.value ? "yes" : "no"
            }
        }

        TableViewColumn {
            role: "id" ;
            title: "Tx hash" ;
            width: 750;
        }



    }


}
