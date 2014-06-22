import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1


AbstractPage {

    ColumnLayout {

//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.top: parent.top
//        anchors.fill: parent
        anchors.topMargin: 45
    anchors.centerIn: parent

        Label {
            text: "Choose an import method.\n Your can either import an existing wallet (file)\nor use a previoulsy generated seed."
        }

        RowLayout {

            Button {
                text: "Import Local Wallet"
                onClicked: goTo(importWalletPage)
            }

            Button {
                enabled: wallet_handler.seed_available
                text: "Recover from Seed"
                onClicked: goTo(recoverWalletPage)
            }

//            Component.onCompleted: {
//                if (!wallet_handler.seed_available) {
//                    goTo(importWalletPage);
//                }
//            }
        }

    }

}
