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

        text: qsTr("Please confirm your wallet password to continue")

    }

    PasswordTextField {
        id: passwordConfirmationInput

        placeholderText: qsTr("Confirm your password", "confirm password placeholder")

    }

    Button {

        text: qsTr("Confirm password", "confirm password button")
        onClicked: {
            createWalletError = "";

            if (passwordConfirmationInput.text !== passwordToConfirm) {

                createWalletError = qsTr("Wrong password");
                return;
            }

            var res = wallet_handler.createWallet(walletFileToCreate, passwordToConfirm, seedToUse);
            switch(res) {
                case 0:

                    settings.wallet_file = walletFileToCreate;
                    settings.wallet_password = passwordToConfirm;
                    goToNext();
                    return;
                case 1:
                    createWalletError =  qsTr("File error : Invalid wallet name or path.")
                    break;
                case 2:
                    createWalletError =  qsTr("Password error (too short ?).")
                    break;
                case 3:
                    createWalletError =  qsTr("Invalid seed. Please restart with a proper seed.")
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
