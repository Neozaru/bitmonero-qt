import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


GuardedColumnLayout {

    anchors.fill: parent
    anchors.margins: 10

    ColumnLayout {
        id: balanceLayout

        Label {
            text: "Balance : "
        }

        Balance {
            balance: wallet.balance
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



