import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "Common"
import "qrc:/qml/Utils"

GuardedColumnLayout {
    id: transferLayout

    property string lastTransferError: ""
    property int defaultFee: 1000000

    Connections {
        target: wallet
        onTransferError: {
            console.log("TRANSFER ERROR")
            var additional_info = "";
            if ( error_message == "transaction was rejected by daemon" ) {
                additional_info = "Please wait for network syncing.";
            }

            transferLayout.lastTransferError = "["+error_message+"] " + additional_info;
        }
    }


    anchors.fill: parent
    anchors.margins: 10

    Button {
        id: pasteInputAddress

        text: "Paste recipient address"
        onClicked: { inputAddress.text = ""; inputAddress.paste() }
    }

    AddressTextField {
        id: inputAddress

        anchors.top: pasteInputAddress.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        placeholderText: "Recipient address"

        focus: true;

        onTextChanged: lastTransferError = ""

    }

    CheckBox {
        id: definePaymentIdCheckbox

        anchors.top: inputAddress.bottom
        anchors.topMargin: 10

        text: "Define a payment ID for this transfer (useful for services/exchanges)"
        checked: false
    }

    RowLayout {
        id: paymentIDLayout

        anchors.top: definePaymentIdCheckbox.bottom
        anchors.topMargin: 3
        anchors.left: parent.left
        anchors.right: parent.right
        visible: definePaymentIdCheckbox.checked

        Button {
            id: pastePaymentIDButton

//            anchors.top: definePaymentIdCheckbox.bottom


            text: "Paste"
            onClicked: { inputPaymentId.text = ""; inputPaymentId.paste() }
        }

        TextField {
            id: inputPaymentId

            anchors.left: pastePaymentIDButton.right
            anchors.right: parent.right

            placeholderText: "Payment ID"

            validator: RegExpValidator{ regExp: /[a-zA-Z0-9]{64}/ }
            textColor: acceptableInput ? "green" : "red"
        }

    }

    TextField {
        id: inputAmount

        anchors.top: paymentIDLayout.bottom
        anchors.topMargin: 5

        placeholderText: "0.00"
        onTextChanged: lastTransferError = ""

        validator: DoubleValidator { bottom: 0.0; top: 100000000 }

        font.pixelSize: 20
    }


    Button {
        id: buttonSend

        anchors.left: inputAmount.right
        anchors.top: inputAmount.top
        anchors.leftMargin: 3

//        text: "Send"
        enabled: inputAmount.acceptableInput && inputAddress.acceptableInput

        onClicked:  if (!inputAmount.acceptableInput) {
                        lastTransferError = "Please enter an amount to send";
                    }
                    else if (!inputAddress.acceptableInput) {
                        lastTransferError = "Please enter a recipient address";
                    }
                    else if (inputPaymentId.text.length != 0 && !inputPaymentId.acceptableInput) {
                        lastTransferError = "Please enter a valid payment ID (64 chars) or disable it";
                    }
                    else {

                        var amount = Math.pow(10,12) * parseFloat(inputAmount.text)
                        var fees = customFeesCheckbox.checked && customFeesInput.acceptableInput ? Math.pow(10,12) * parseFloat(customFeesInput.text) : transferLayout.defaultFee

                        console.log("Transfer")
                        console.log("Amount :" + amount)
                        console.log("Fees : " + fees)
                        var res = wallet.transfer(amount, inputAddress.text, fees, definePaymentIdCheckbox ? inputPaymentId.text : "");
                        lastTransactionLayout.visible = false;
                    }

        style: ButtonStyle {

            label: Label {
                font.pixelSize: 18
                text: "SEND"
            }

        }

    }

    RowLayout {
        anchors.top: buttonSend.bottom
        anchors.topMargin: 5

        CheckBox {
            id: customFeesCheckbox

            text: "Custom network fee : "
        }

        TextField {
            id: customFeesInput
            anchors.left: customFeesCheckbox.right

            text: (Math.pow(10,-12) * transferLayout.defaultFee).toFixed(12)

            visible: customFeesCheckbox.checked
            placeholderText: (Math.pow(10,-12) * transferLayout.defaultFee).toFixed(12)

            validator: DoubleValidator { bottom: 0.0; top: 1 }

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

            inputAmount.text = ""
            inputAddress.text = ""
            lastTransactionHash.text = tx_hash
            lastTransactionLayout.visible = true;

            transferLayout.lastTransferError = ""

        }

    }

    Text {
        id: errorMessage
        color: "red"
        text: lastTransferError
    }






}
