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

        ColumnLayout {
            id: balanceLayout

            Label {
                text: "Balance : "
            }

            TextEdit {
                readOnly: true
                id: balanceValue
                text: (wallet.balance * Math.pow(10,-12)).toFixed(12)
            }

            Button {
                text: "Copy balance"
                /* Hack copy */
                onClicked: { balanceValue.selectAll(); balanceValue.copy(); balanceValue.select(0,-3)}
            }

       }

       ColumnLayout {

           anchors.top: balanceLayout.bottom
           anchors.topMargin: 20

           Label {
               text: "Your Monero address : "
           }

           TextEdit {
               id: addressValue

               readOnly: true
               text: wallet.address
           }

           Button {
               text: "Copy address"
               /* Hack copy */
               onClicked: { addressValue.selectAll(); addressValue.copy(); addressValue.select(0,0)}
           }
       }




    }

}



