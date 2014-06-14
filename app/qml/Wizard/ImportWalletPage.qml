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

                text: "Use default Wallet location"
                checked: true
            }


            FileDialog {
                id: fileDialog
                title: "Please choose a file"
                selectFolder: true
                selectExisting: true

                onAccepted: {
                    console.log("You chose: " + fileDialog.fileUrls)
//                    walletLocationInput.text = fileDialog.folder
                    importWalletLayout.walletLocation = fileDialog.folder.toLocaleString()

                    wallet_handler.findWallets(importWalletLayout.walletLocation)
                    walletsTable.selection.select(0)

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

                    text: "Choose your wallet file (can be located in .bitmonero/)"
                }

                RowLayout {

                    anchors.left: parent.left
                    anchors.right: parent.right

                    Button {
                        id: browserFolderButton

                        text: "Browse"

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

            anchors.top: walletNameLayout.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            alternatingRowColors: true

            model: wallet_handler.last_found_wallets

            TableViewColumn {

                role: "name";
                title: "Name";
                width: 120

            }

            TableViewColumn {

                role: "status" ;
                title: "Status" ;
                width: 60;

                delegate: Text {
                    property bool sane: parseInt(styleData.value) === 0;

                    text: sane ? "OK" : "To recover"
                    color: sane ? "green" : "orange"
                }
            }

            TableViewColumn{

                role: "address";
                title: "Address" ;
                width: 400;

            }


            Component.onCompleted: {

                wallet_handler.findWallets()
                selection.select(0)
            }


        }


        ColumnLayout {
            id: walletPasswordLayout

            anchors.top: walletsTable.bottom
            anchors.topMargin: 3
            anchors.right: parent.verticalCenter

            Label {
                id: walletPasswordLabel

                text: "Password of your existing wallet"

            }

            PasswordTextField {
                id: walletPasswordInput

                placeholderText: "Password of your Wallet"
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
//            anchors.bottomMargin: 5

            text: "Import Wallet"
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
                        importWalletError = "No wallet selected"
                        return;
                    }

                    var location = wallet_handler.last_found_wallets[walletsTable.currentRow].file_path;


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


