import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "qrc:/qml/Utils/"

import "Common"

GuardedColumnLayout {

    anchors.fill: parent

    Image {
        id: overviewImage

        anchors.margins: 10

//        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        source: "../res/MoneroLogoSquare.png"
        fillMode: Image.PreserveAspectFit
        smooth: true

        width: 100
        height: 100
    }

    ColumnLayout {
        id: balanceLayout

        anchors.top: parent.top
        anchors.topMargin: overviewImage.height / 2

        Label {
            text: qsTr("You have", "you have xxx XMR") + " : "
            font.pixelSize: 18

        }

        Balance {
            balance: wallet.balance
            font.pixelSize: 24
        }

        RowLayout {
            visible: wallet.balance != wallet.unlocked_balance

            Label {
                text: qsTr("Usable right now", "you have xxx XMR unlocked") + " : "
            }

            Balance {
                balance: wallet.unlocked_balance
            }

        }

        Button {
            text: qsTr("Copy balance")
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
           text: qsTr("Your Monero address") + " : "

           font.pixelSize: 18

       }

       TextEdit {
           id: addressValue

           readOnly: true
           text: wallet.address

           color: "#0A6300"

       }

       Button {
           text: qsTr("Copy address")
           /* Hack copy */
           onClicked: { addressValue.selectAll(); addressValue.copy(); addressValue.select(0,0)}


       }
   }


}



