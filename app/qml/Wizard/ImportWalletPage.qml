import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "qrc:/qml/Utils"

AbstractPage {

    property string importWalletError;


    ColumnLayout {
        id: importWalletLayout

        property string walletLocation;

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
//        anchors.fill: parent
        anchors.topMargin: 45
//        anchors.bottom: parent.bottom

        ColumnLayout {
            id: walletNameLayout

            CheckBox {
                id: useDefaultImportLocationCheckbox
                anchors.bottomMargin: 5

                text:  qsTr("Use default Wallet location", "wallet location checkbox")
                checked: true
            }


            FileDialog {
                id: fileDialog
                title:  qsTr("Please choose a file", "file chooser title")
                selectFolder: true
                selectExisting: true

                onAccepted: {
                    console.log("You chose: " + fileDialog.fileUrls)
//                    walletLocationInput.text = fileDialog.folder
                    importWalletLayout.walletLocation = fileDialog.folder.toLocaleString()

//                    wallet_handler.findWallets(importWalletLayout.walletLocation)
                    walletsTable.selection.select(0)
                    walletsTable.updateWalletsList(importWalletLayout.walletLocation)

                }
                onRejected: {
                    console.log("Canceled")
                }

                visible: false
            }


            ColumnLayout {

                visible: !useDefaultImportLocationCheckbox.checked

                Label {
                    id: walletFromFileLabel

                    text:  qsTr("Choose your wallet file (can be located in %1)").arg(".bitmonero/")
                }

                RowLayout {

                    anchors.left: parent.left
                    anchors.right: parent.right

                    Button {
                        id: browserFolderButton

                        text: qsTr("Browse", "Exec file chooser")

                        onClicked: fileDialog.visible = true
                    }

                    Label {
                        id: walletLocationInput

                        anchors.left: browserFolderButton.right
                        anchors.leftMargin: 5
                        anchors.right: parent.right
                        text: importWalletLayout.walletLocation

                    }

                }

            }
        }


        TableView {
            id: walletsTable


            property var last_found_wallets: []

            height: 300
            anchors.top: walletNameLayout.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            alternatingRowColors: true

            model: last_found_wallets

            TableViewColumn {

                role: "name";
                title: qsTr("Name", "wallet list name");
                width: 120

            }

            TableViewColumn {

                role: "status" ;
                title: qsTr("Status", "wallet list status");
                width: 60;

                delegate: Text {
                    property bool sane: parseInt(styleData.value) === 0;

                    text: sane ? qsTr("OK", "wallet status OK") : qsTr("To recover", "wallet status To recover")
                    color: sane ? "green" : "orange"
                }
            }

            TableViewColumn{

                role: "address";
                title: qsTr("Address", "wallet list address");
                width: 400;

            }

            function updateWalletsList(location) {

                walletsTable.last_found_wallets = wallet_handler.findWallets(location)
                selection.select(0)

            }

            Component.onCompleted: {
                updateWalletsList(wallet_handler.default_wallet_location)
            }


        }


        ColumnLayout {
            id: walletPasswordLayout

            anchors.top: walletsTable.bottom
            anchors.topMargin: 3
            anchors.right: parent.horizontalCenter
            anchors.left: parent.left

            Label {
                id: walletPasswordLabel

                text: qsTr("Password of your existing wallet", "existing password label")

            }

            PasswordTextField {
                id: walletPasswordInput

                placeholderText: qsTr("Password of your Wallet", "existing password placeholder")
            }

        }

        function areAcceptableInputs() {
            return ;
        }


        Button {
            id: importWalletButton

            anchors.top: walletPasswordLayout.bottom
            anchors.topMargin: 10
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: 10

            text: qsTr("Import Wallet", "confirm import wallet button")
            action: importWalletAction

        }

        Action {
            id: importWalletAction
            shortcut: "Ctrl+I"

            enabled: walletsTable.currentRow != -1 && walletPasswordInput.acceptableInput

            onTriggered: {

                if (importWalletButton.enabled) {

                    console.log("Row : " + walletsTable.currentRow)

                    if (walletsTable.currentRow < 0) {
                        importWalletError = qsTr("No wallet selected", "error no wallet selected")
                        return;
                    }

                    var location = walletsTable.last_found_wallets[walletsTable.currentRow].file_path;


                    settings.setWalletFile(location);
                    settings.setWalletPassword(walletPasswordInput.text)

                    console.log("Set wallet configuration : ")
                    console.log("File : " + location )
                    console.log("Password : xxx (" + walletPasswordInput.text.length + "chars)")

                    goToNext();

                }
            }
        }

        Label {
            text: importWalletError
            color: "red"
        }

    }
}


