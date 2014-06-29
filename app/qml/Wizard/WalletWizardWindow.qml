import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Controls.Styles 1.1

import "qrc:/qml/Utils"

ApplicationWindow {
    id: walletWizardWindow


    signal walletCreated();
    signal walletImported();
    signal wizardSuccess();

    color: "#FAFAFA"

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

            Image {
                id: wizardImage

                anchors.margins: 10
                anchors.left: parent.left
                anchors.right: parent.right

                source: "../res/MoneroLogo.png"
                fillMode: Image.PreserveAspectFit
                smooth: true
            }

            Label {
                id: welcomeLabel

                anchors.horizontalCenter: parent.horizontalCenter

                text: qsTr("Welcome to Monero Wallet.") + "\n" +
                       qsTr("Your Wallet is not configured yet") + qsTr("or", "wallet not configured <or> first time") + "\n" +
                       qsTr("it is the first time you launch the application.") + "\n" +
                       qsTr("Choose an option below to get started")

                font.pixelSize: 14
            }


            RowLayout {

                anchors.top: welcomeLabel.bottom;
                anchors.topMargin: 10

                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: qsTr("Create new wallet", "create new wallet button")
                    onClicked: {
                        walletSeed = "";
                        myStack.push(createWalletPage)
                    }

                }

                Button {
                    text: qsTr("Use existing wallet", "use existing wallet button")
                    onClicked: { myStack.push(importWalletChoicePage) }

                }
            }
        }

    }

    property string walletFilename
    property string walletPassword
    property string walletSeed

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
            seedToUse: walletSeed
        }

    }

    Component {
        id: importWalletChoicePage

        ImportWalletChoicePage {

            stack: myStack
            nextPage: importWalletChoicePage
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
        id: recoverWalletPage

        RecoverWalletPage {

            stack: myStack
            nextPage: createWalletPage


            onWalletSeedSet: {
                walletSeed = seed;
            }

        }

    }


    Component {
        id: successPage

        ColumnLayout {
            id: statusLayout

            property int status: 0;

            anchors.centerIn: parent


            ColumnLayout {
                visible: status == 0

                Label {
                    text: qsTr("Checking configuration...") + "\n" + qsTr("This one-time operation can take several minutes.")
                }

                ProgressBar {
                    indeterminate: true
                }
            }

            ColumnLayout {
                visible: status == 1

                Label {
                    text: qsTr("Monero Wallet was successfully configured.") + "\n" + qsTr("You can now start using your Wallet.")
                }

                ColumnLayout {
                    visible: wallet_handler.ephemeral_seed.length !== 0

                    Label {
                        text: qsTr("Please memorize or store this seed in a SAFE place (wallet recovery)")

                        color: "red"
                        font.pixelSize: 12
                    }

                    Button {
                        text: qsTr("Copy seed")
                        onClicked: {
                            seedDisplayTextArea.selectAll()
                            seedDisplayTextArea.copy();
                            seedDisplayTextArea.select(0,0)
                        }
                    }

                    TextArea {
                        id: seedDisplayTextArea

                        anchors.left: parent.left
                        anchors.right: parent.right

                        readOnly: true
                        text: wallet_handler.ephemeral_seed
                    }

                }


                Button {
                    text: qsTr("Go !", "finish wizard")

                    onClicked: {
                        wallet_handler.ephemeral_seed = "";
                        wizardSuccess();
                    }
                }

            }

            property string tryWalletError: "";
            ColumnLayout {
                visible: status == -1

                Label {
                    text: statusLayout.tryWalletError
                    color: "red"
                }

                Button {
                    text: qsTr("Retry", "retry after an error (button)")

                    onClicked: { myStack.clear(); myStack.push(myStack.initialItem) }
                }

            }

            Component.onCompleted: {

                connectionWalletHandler.target = wallet_handler;
                status = 0;
                if (!wallet_handler.tryWalletAsync(settings.wallet_file,settings.wallet_password)) {
                    /* Check if status is not already returned. Otherwise return generic error */
                    if (status == 0) {
                        statusLayout.tryWalletError = qsTr("Unknown error with wallet handler")
                        status = -1
                    }

                }

            }



            Connections {
                id: connectionWalletHandler
                target: null
                onTryWalletResult: {
                    console.log("Try Wallet Result " + result);

                    connectionWalletHandler.target = null;

                    if ( result == 0 ) {
                        status = 1
                        console.log("Try Wallet returned good status")
                    }
                    else {
                        console.log("Try Wallet returned BAD status")

                        switch(result) {
                            case 1:
                                statusLayout.tryWalletError = qsTr("File error : Invalid wallet name or path.")
                                break;
                            case 2:
                                statusLayout.tryWalletError = qsTr("Error : Wrong password")
                                break;
                            default:
                                statusLayout.tryWalletError = qsTr("Unknown error (%1)").arg(createWalletError)
                                break;

                        }

                        status = -1
                        /* Rollback configuration */
                        settings.wallet_file = "";
                        settings.wallet_password = "";
                    }

                }

            }


        }
    }


    onWizardSuccess: {
        visible = false;
        application.notifyWizardSuccess();
//        Qt.quit();
    }


}
