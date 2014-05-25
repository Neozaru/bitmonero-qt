import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


RowLayout {


    property string lastError: ""
    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 10

        Button {
            id: pasteInputAddress
            text: "Paste recipient address"
            onClicked: { inputAddress.text = ""; inputAddress.paste() }
        }

//        TextField {

//            anchors.left: parent.left
//            anchors.right: parent.right
//            id: inputAddress

//            placeholderText: "Recipient address"
//            maximumLength: 95

//            onTextChanged: lastError = ""
//            textColor: if (acceptableInput) { "green" } else { "red" }

//            validator: RegExpValidator { regExp: /[a-zA-Z0-9]{95}/ }
//        }

        AddressTextField {

            id: inputAddress
            anchors.left: parent.left
            anchors.right: parent.right

            placeholderText: "Recipient address"

            onTextChanged: lastError = ""

        }

        TextField {
            id: inputAmount

            placeholderText: "0.00"
            onTextChanged: lastError = ""

            validator: DoubleValidator { bottom: 0.0; top: 100000000 }
        }

        Button {
            id: buttonSend
            text: "Send"
            enabled: inputAmount.acceptableInput && inputAddress.acceptableInput

            onClicked:  if (!inputAmount.acceptableInput) {
                            lastError = "Please enter an amount to send";
                        }
                        else if (!inputAddress.acceptableInput) {
                            lastError = "Please enter a recipient address";
                        }
                        else {
                            var res = wallet.transfer(Math.pow(10,12) * parseFloat(inputAmount.text), inputAddress.text);
                            lastTransactionLayout.visible = false;
                        }

        }

        ColumnLayout {
            id: lastTransactionLayout
            visible: false;

            Label {
                text: "Transaction successful :"
                color: "green"
            }

            RowLayout {

                Button {
                    text: "Copy Hash"
                    onClicked: { lastTransactionHash.selectAll(); lastTransactionHash.copy(); lastTransactionHash.select(0,0) }
                }

                TextEdit {
                    id: lastTransactionHash

                    readOnly: true
                }

            }
        }


        Connections {
            target: wallet
            onTransferSuccessful: {

                inputAmount.text = 0
                inputAddress.text = ""
                lastTransactionHash.text = tx_hash
                lastTransactionLayout.visible = true;

            }

        }

        Text {
            id: errorMessage
            color: "red"
            text: lastError
        }


    }



}
