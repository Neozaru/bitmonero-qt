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

    ListModel {


        id: transactionsListModel
        ListElement {
            type: "R"
            time: "02/03/14 22:65:98"
            amount: "16.65464887"
            origin: "48YCiaYuvKSABq4Fj37iMmDeKWcnqCMd2PuoJVMUYCRnFcUFq1QKXjqJ8dg3PCvroJLtY86besAxaGVkzAArvHTiUAUcDHG"
            destination: "48YCiaYuvKSABq4Fj37iMmDeKWcnqCMd2PuoJVMUYCRnFcUFq1QKXjqJ8dg3PCvroJLtY86besAxaGVkzAArvHTiUAUcDHG"
//            origin_display: if (origin_username) { origin_username } else { destination }

        }

        ListElement {
            type: "R"
            time: "02/02/14 22:20:02"
            amount: "10.68462"
            origin: "48YCiaYuvKSABq4Fj37iMmDeKWcnqCMd2PuoJVMUYCRnFcUFq1QKXjqJ8dg3PCvroJLtY86besAxaGVkzAArvHTiUAUcDHG"
            destination: "48YCiaYuvKSABq4Fj37iMmDeKWcnqCMd2PuoJVMUYCRnFcUFq1QKXjqJ8dg3PCvroJLtY86besAxaGVkzAArvHTiUAUcDHG"
            origin_username: "Neozaru"
//            origin_display: origin_username && origin_username || destination
        }

    }


    property bool iamVisible: transactionsTable.currentRow == 0

    Button {
        id: copySenderButton
        text: "Copy sender address"
        enabled: transactionsTable.currentRow >= 0
        onClicked: {
            var origin = transactionsTable.model.get(transactionsTable.currentRow)["origin"];
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

        anchors.top: copySenderButton.bottom
        anchors.topMargin: 5

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: transactionsListModel
        TableViewColumn{ role: "type"  ; title: "Type" ; width: 45 }
        TableViewColumn{ role: "time"  ; title: "Time" ; width: 150 }
        TableViewColumn{ role: "amount" ; title: "Amount" ; width: 100; }
        TableViewColumn{ role: "origin_username" ; title: "User" ; width: 100; }
        TableViewColumn{ role: "origin" ; title: "Address From" ; width: 750; }

        onDoubleClicked: console.log(row)
//        Too large atm
//        TableViewColumn{ role: "destination" ; title: "Address To (your wallet)" ; width: 200 }

    }


}
