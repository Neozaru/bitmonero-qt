import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "qrc:/qml/Utils/"

import "Common"

GuardedColumnLayout {

    anchors.fill: parent

    ColumnLayout {
        id: balanceLayout

        Label {
            text: "You have : "
            font.pixelSize: 18

        }

        Balance {
            balance: wallet.balance
            font.pixelSize: 24
        }

        RowLayout {
            visible: wallet.balance != wallet.unlocked_balance

            Label {
                text: "Usable right now : "
            }

            Balance {
                balance: wallet.unlocked_balance
            }

        }

        Button {
            text: "Copy balance"
            /* Hack copy */
            onClicked: { balanceValue.selectAll(); balanceValue.copy(); balanceValue.select(0,0)}
        }



        /* hack */
        TextEdit {
            id: balanceValue
            visible: false

            text: Math.pow(10,-12)*wallet.balance
        }


   }


   ColumnLayout {

       anchors.top: balanceLayout.bottom
       anchors.topMargin: 20

       Label {
           text: "Your Monero address : "

           font.pixelSize: 18

       }

       TextEdit {
           id: addressValue

           readOnly: true
           text: wallet.address

           color: "#0A6300"

       }

       Button {
           text: "Copy address"
           /* Hack copy */
           onClicked: { addressValue.selectAll(); addressValue.copy(); addressValue.select(0,0)}


       }
   }






}



