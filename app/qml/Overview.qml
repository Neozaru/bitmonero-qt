import QtQuick 2.0
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


RowLayout {

    property variant ww : wallet

    Layout.minimumWidth: implicitWidth
    Layout.fillWidth: false


    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 10

        Layout.minimumWidth: implicitWidth
        Layout.fillWidth: false

        Label {
            text: "Balance : "
        }

        TextEdit {
            readOnly: true
            id: balanceValue
            text: (wallet.balance * Math.pow(10,-12)).toFixed(12)
        }

        Button {
            text: "Copy"
            /* Hack copy */
            onClicked: { balanceValue.selectAll(); balanceValue.copy(); balanceValue.select(0,0)}
//
        }

        Label {
           id: testLabel
//               text: (wallet.transferSuccessful && "no" || "ok")
//               onTransferSuccessful: text = "OK"

        }


    }

}



