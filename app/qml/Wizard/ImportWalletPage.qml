import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "qrc:/qml/Utils"

AbstractPage {

    property string importWalletError;
    ColumnLayout {
        id: importWalletLayout


        CheckBox {
            id: useDefaultImportLocationCheckbox

            text: "Use default Wallet location"
            checked: true
        }

        ColumnLayout {
            visible: useDefaultImportLocationCheckbox.checked

            Label {
                id: walletNameLabel

                text: "Name of your wallet"
            }

            TextField {
                id: walletNameInput

                placeholderText: "Name of your Wallet"
                textColor: if (acceptableInput) { "green" } else { "red" }
                validator: RegExpValidator { regExp: /[a-zA-Z0-9\.]+/ }
            }
        }




        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                walletLocationInput.text = fileDialog.fileUrl
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


                TextField {
                    id: walletLocationInput

                    placeholderText: "Wallet location"

                }

                Button {
                    text: "Browse"

                    onClicked: fileDialog.visible = true
                }
            }

        }



        ColumnLayout {

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
            text: "Import Wallet"

//            enabled: ((
//                          useDefaultImportLocationCheckbox.checked && walletNameInput.acceptableInput ||
//                          walletLocationInput.length > 0
//                      )
//                      && walletPasswordInput.acceptableInput)


            action: importWalletAction
//            onClicked: {

//                if (true) {

//                    var default_location = wallet_handler.default_wallet_location;

//                    var location = useDefaultImportLocationCheckbox.checked ? default_location + walletNameInput.text : walletLocationInput.text.replace('file://','');

//                    settings.setWalletFile(location);
//                    settings.setWalletPassword(walletPasswordInput.text)

//                    goToNext();

//                }

//            }
        }

        Action {
            id: importWalletAction
            shortcut: "Ctrl+I"
            enabled: (useDefaultImportLocationCheckbox.checked || walletNameInput.acceptableInput ) && walletPasswordInput.acceptableInput

            onTriggered: {

                if (importWalletButton.enabled) {

                    var default_location = wallet_handler.default_wallet_location;

                    var location = useDefaultImportLocationCheckbox.checked ? default_location + walletNameInput.text : walletLocationInput.text.replace('file://','');

                    settings.setWalletFile(location);
                    settings.setWalletPassword(walletPasswordInput.text)

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


