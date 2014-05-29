import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "qrc:/qml/Utils"

ApplicationWindow {
    id: walletWizardWindow

    signal walletCreated();
    signal walletImported();
    signal wizardSuccess();

    StackView {
        id: myStack

        anchors.fill: parent;
        anchors.margins: 10;

        delegate: StackViewDelegate {
            function transitionFinished(properties)
            {
                properties.exitItem.opacity = 1
            }

            pushTransition: StackViewTransition {
                PropertyAnimation {
                    target: enterItem
                    property: "opacity"
                    from: 0
                    to: 1
                }
                PropertyAnimation {
                    target: exitItem
                    property: "opacity"
                    from: 1
                    to: 0
                }
            }
        }

        initialItem: welcomePage

    }

//    myStack.initialItem: welcomePage

    Component {
        id: welcomePage

        ColumnLayout {

            anchors.centerIn: parent;
            anchors.horizontalCenter: parent.horizontalCenter


            Label {
                id: welcomeLabel

                text: "Welcome to Monero Wallet." + "\n" +
                       "Your Wallet is not configured yet or it is " + "\n" +
                       "the first time you launch the application." + "\n" +
                       "Choose an option below to get started"
            }


            RowLayout {

                anchors.top: welcomeLabel.bottom;
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: "Create new wallet"
                    onClicked: { myStack.push(createWalletPage) }
                }

                Button {
                    text: "Use existing wallet"
                    onClicked: { myStack.push(importWalletPage) }

                }
            }
        }

    }

    property string walletFilename
    property string walletPassword

    Component {
        id: createWalletPage

        CreateWalletPage {
            id: createWalletContent

            stack: myStack
            nextPage: confirmCreateWalletPage

            onWalletFilenameSet: {
                walletFilename = filename;
            }

            onWalletPasswordSet: {
                walletPassword = password;
            }

        }



    }


    Component {
        id: confirmCreateWalletPage

        ConfirmCreateWalletPage {

            stack: myStack
            nextPage: successPage

            walletFileToCreate: walletFilename
            passwordToConfirm: walletPassword
        }

//        Component.onCompleted: {  }

    }

    Component {
        id: importWalletPage

        ImportWalletPage {

            stack: myStack
            nextPage: successPage

        }

    }

    Component {
        id: successPage

        ColumnLayout {

            Label {
                text: "Monero Wallet was successfully configured.\n You can now start using your Wallet"
            }

            Button {
                text: "Go !"

                onClicked: { wizardSuccess() }
            }


        }
    }


}
