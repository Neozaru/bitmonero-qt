import QtQuick 2.0
import QtQuick.Controls 1.1


TableView {

    alternatingRowColors: true
    sortIndicatorColumn: 1
    sortIndicatorOrder: Qt.DescendingOrder
    sortIndicatorVisible: true

//    model: (mainWindow.advancedInterface && advancedViewCheckbox.checked) ? wallet.transactions : wallet.aggregated_transactions


    TableViewColumn {
        role: "block_height" ;
        title: qsTr("Block", "(tx list)");
        width: 80;
    }

//    TableViewColumn {
//        role: "date" ;
//        title: qsTr("Date", "(tx list)");
//        width: 120;


//        delegate: Text {
//            text: Qt.formatDateTime(styleData.value)
//        }
//    }

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
