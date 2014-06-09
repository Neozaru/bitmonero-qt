import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "qrc:/qml/Utils/"

import "Common"

GuardedColumnLayout {

    anchors.fill: parent
//    anchors.margins: 10

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

//    Image {
////        anchors.top: balanceLayout.top
////        anchors.left: balanceLayout.right
////        anchors.right: parent.right

//        anchors.right: parent.right
//        anchors.bottom: parent.bottom

//        source: 'qrc:/qml/res/MoneroLogoSmall.png'

//        fillMode: Image.PreserveAspectFit
//        smooth: true

////        height: 90
////        width: 90
//    }

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

//           color: "green"
           color: "#0A6300"

       }

       Button {
           text: "Copy address"
           /* Hack copy */
           onClicked: { addressValue.selectAll(); addressValue.copy(); addressValue.select(0,0)}

            ButtonStyle {
                label: Label {
                    text: parent.text
                    color: "green"

                }
            }

       }
   }






}



