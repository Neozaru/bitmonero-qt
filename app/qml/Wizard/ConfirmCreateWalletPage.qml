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
            if (passwordConfirmationInput.text !== passwordToConfirm) {

                createWalletError = "Wrong password";
                return;
            }

            var res = wallet_handler.createWallet(walletFileToCreate, passwordToConfirm, seedToUse);
            if ( res ) {

                settings.wallet_file = walletFileToCreate;
                settings.wallet_password = passwordToConfirm;
                goToNext();


            }
            else {
                createWalletError = "An error has occurred while creating your Wallet";
            }


        }


    }


    Label {
        text: createWalletError
        color: "red"
    }


}
