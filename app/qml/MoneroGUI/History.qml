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

    Button {
        id: copyTxHashButton
        visible: false
        text: "Copy Tx Hash"
        enabled: transactionsTable.currentRow >= 0
        onClicked: {
            var origin = transactionsTable.model.get(transactionsTable.currentRow)["id"];
            console.log(origin)
            /* TODO : Make a C++ interface for clipboard in order to remove this UGLY hack */
            stubCopyPasteTextEdit.text = origin;
            stubCopyPasteTextEdit.selectAll();
            stubCopyPasteTextEdit.copy();
            stubCopyPasteTextEdit.text = "";
        }
    }


    TableView {

        id: transactionsTable

        anchors.top: copyTxHashButton.bottom
        anchors.topMargin: 5

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        alternatingRowColors: true


        model: wallet.transactions
        TableViewColumn {

            role: "type";
            title: "Type";
            width: 45

            delegate: Text {
                text: styleData.value ? styleData.value : " ?"
            }
        }
//        TableViewColumn{ role: "time"  ; title: "Time" ; width: 150 }
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

//        section.property: "id"
//        section.delegate: sectionHeading


//        onDoubleClicked: console.log(row)

    }


}
