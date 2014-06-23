import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "qrc:/qml/Utils"

AbstractPage {

    property string passwordToConfirm;
    property string walletFileToCreate;
    property string seedToUse;

    property string createWalletError;

    anchors.centerIn: parent;

    Label {

        text: "Please confirm your wallet password to continue"

    }

    PasswordTextField {
        id: passwordConfirmationInput

        placeholderText: "Confirm your password"

    }

    Button {

        text: "Confirm password"
        onClicked: {
            createWalletError = "";

            if (passwordConfirmationInput.text !== passwordToConfirm) {

                createWalletError = "Wrong password";
                return;
            }

            var res = wallet_handler.createWallet(walletFileToCreate, passwordToConfirm, seedToUse);
            switch(res) {
                case 0:

                    settings.wallet_file = walletFileToCreate;
                    settings.wallet_password = passwordToConfirm;
                    goToNext();
                    return;

//                    break;
                case 1:
                    createWalletError = "File error : Invalid wallet name or path."
                    break;
                case 2:
                    createWalletError = "Password error (too short ?)."
                    break;
                case 3:
                    createWalletError = "Invalid seed. Please restart with a proper seed."
                    break;
                default:
                    createWalletError = qsTr("Unknown error (%1)").arg(res)
                    break;

            }



        }


    }


    Label {
        text: createWalletError
        color: "red"
    }


}
