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
    property real defaultFee: 5000000000

    anchors.fill: parent
//    anchors.margins: 10
//    anchors.top: parent.top

    Connections {
        target: wallet
        onTransferError: {
            console.log("TRANSFER ERROR")
            var additional_info = "";
            if ( error_message === "transaction was rejected by daemon" ) {
                additional_info = "Please wait for network syncing.";
            }

            transferLayout.lastTransferError = "["+error_message+"] " + additional_info;
        }
    }


    Button {
        id: pasteInputAddress

        anchors.top: parent.top
        anchors.topMargin: 20

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

        validator: DoubleValidator {
            bottom: 0.0;
            top: 100000000;
            notation:DoubleValidator.StandardNotation;
            locale: "en_US"
        }


        font.pixelSize: 20
    }

    property bool amountUsesComma: inputAmount.text.indexOf(",") != -1

    Button {
        id: buttonSend

        anchors.left: inputAmount.right
        anchors.top: inputAmount.top
        anchors.leftMargin: 3

//        text: "Send"
        enabled: inputAmount.acceptableInput && inputAddress.acceptableInput && !amountUsesComma

        onClicked:  if (!inputAmount.acceptableInput && parseFloat(inputAmount) !== 0 && !amountUsesComma) {
                        lastTransferError = "Please enter an amount to send";
                    }
                    else if (!inputAddress.acceptableInput) {
                        lastTransferError = "Please enter a recipient address";
                    }
                    else if (inputPaymentId.text.length != 0 && !inputPaymentId.acceptableInput) {
                        lastTransferError = "Please enter a valid payment ID (64 chars) or disable it";
                    }
                    else {

                        var text_amount = inputAmount.text
                        var parsed_amount = parseFloat(inputAmount.text)
                        var amount_mini = Math.pow(10,12) * parsed_amount

                        var text_fees = customFeesInput.text
                        var parsed_fees = parseFloat(customFeesInput.text)
                        var fees_mini = Math.pow(10,12) * parsed_fees

//                        var fees = (customFeesCheckbox.checked && customFeesInput.acceptableInput) ? (Math.pow(10,12) * parseFloat(customFeesInput.text)) : transferLayout.defaultFee

                        var fees_to_apply = (customFeesCheckbox.checked && customFeesInput.acceptableInput) ? fees_mini : defaultFee
                        console.log("Transfer")
                        console.log("Amount :" + amount_mini + "(" + parsed_amount + ")")
                        console.log("Fees : " + fees_mini + "("+parsed_fees+") --> " + fees_to_apply)
                        var res = wallet.transfer(amount_mini, inputAddress.text, fees_to_apply, definePaymentIdCheckbox ? inputPaymentId.text : "");
                        lastTransactionLayout.visible = false;
                    }

        style: ButtonStyle {

            label: Label {
                font.pixelSize: 18
                text: "SEND"
            }

        }

    }

    Label {
        anchors.left: buttonSend.right
        anchors.leftMargin: 10
        anchors.verticalCenter: buttonSend.verticalCenter

        text: ( inputAmount.text.length > 0 ? parseFloat(inputAmount.text) + " XMR" : "" ) + ( amountUsesComma ? " PLEASE USE '.' (dot) instead of ',' (comma) for decimals" : "")
        color: amountUsesComma ? "red" : "green"
    }

    ColumnLayout {
        anchors.top: buttonSend.bottom
        anchors.topMargin: 5

        anchors.left: parent.left
        anchors.right: parent.right

        CheckBox {
            id: customFeesCheckbox

            text: "Custom network fee"
        }

        RowLayout {
            visible: customFeesCheckbox.checked

            anchors.left: parent.left
            anchors.right: parent.horizontalCenter

            Label {
                id: customFeesLabel

                text: "Fee :"
            }

            TextField {
                id: customFeesInput

                anchors.left: customFeesLabel.right
                anchors.leftMargin: 3

                anchors.right: parent.horizontalCenter

                text: (Math.pow(10,-12) * transferLayout.defaultFee).toFixed(12)

                placeholderText: (Math.pow(10,-12) * transferLayout.defaultFee).toFixed(12)

                validator: DoubleValidator { bottom: Math.pow(10,-12) * transferLayout.defaultFee; top: 1 }
            }


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

            console.log("Transfer successful : " + tx_hash)

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
