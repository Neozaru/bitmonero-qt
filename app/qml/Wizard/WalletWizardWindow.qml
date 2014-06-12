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

            property int status: 0;


            ColumnLayout {
                visible: status == 0

                Label {
                    text: "Checking configuration...\nThis one-time operation can take several minutes."
                }

                ProgressBar {
                    indeterminate: true
                }
            }

            ColumnLayout {
                visible: status == 1

                Label {
                    text: "Monero Wallet was successfully configured.\n You can now start using your Wallet"
                }

                Button {
                    text: "Go !"

                    onClicked: { wizardSuccess() }
                }

            }

            ColumnLayout {
                visible: status == -1

                Label {
                    text: "An error has occured while opening your Wallet.\nHave you set the right password ?"
                }

                Button {
                    text: "Retry"

                    onClicked: { myStack.clear(); myStack.push(myStack.initialItem) }
                }

            }

            Component.onCompleted: {


                if (!wallet_handler.tryWalletAsync(settings.wallet_file,settings.wallet_password)) {
                    status = -1
                }


            }

            Connections {
                target: wallet_handler
                onTryWalletResult: {
                    console.log("Try Wallet Result " + result);

                    if ( result ) {
                        status = 1
                        console.log("Try Wallet returned good status")
                    }
                    else {
                        status = -1
                        console.log("Try Wallet returned BAD status")
                        /* Rollback configuration */
                        settings.wallet_file = "";
                        settings.wallet_password = "";
                    }
                }

            }



        }
    }

    onWizardSuccess: {
        visible = false; Qt.quit()

    }


}
