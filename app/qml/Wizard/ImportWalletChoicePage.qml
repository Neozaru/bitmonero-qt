import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1


AbstractPage {

    ColumnLayout {

        anchors {
            topMargin: 45
            centerIn: parent
        }

        Label {
            text:  qsTr("Choose an import method.\n Your can either import an existing wallet (file)\nor use a previoulsy generated seed.")
        }

        RowLayout {

            Button {
                text:  qsTr("Import Local Wallet", "import local wallet button")
                onClicked: goTo(importWalletPage)
            }

            Button {
                enabled: wallet_handler.seed_available
                text:  qsTr("Recover from Seed", "recover wallet button")
                onClicked: goTo(recoverWalletPage)
            }


        }

    }

}
