import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1

import "Common"
import "qrc:/qml/Utils"

GuardedColumnLayout {
    id: transferLayout

    property string lastTransferError: ""

    property bool askConfirmation: false
    property real defaultFee: 5000000000

    anchors.fill: parent


    Connections {
        target: wallet
        onTransferError: {
            console.log("TRANSFER ERROR")
            var additional_info = "";
            if ( error_message === "transaction was rejected by daemon" ) {
                additional_info = qsTr("Please wait for network syncing.");
            }

            transferLayout.lastTransferError = "["+error_message+"] " + additional_info;
        }
    }


    Button {
        id: pasteInputAddress

        anchors.top: parent.top
        anchors.topMargin: 20

        text: qsTr("Paste recipient address")
        onClicked: { inputAddress.text = ""; inputAddress.paste() }
    }

    AddressTextField {
        id: inputAddress

        anchors.top: pasteInputAddress.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        placeholderText: qsTr("Recipient address")

        focus: true;

        onTextChanged: lastTransferError = ""

    }

    CheckBox {
        id: definePaymentIdCheckbox

        anchors.top: inputAddress.bottom
        anchors.topMargin: 10

        text: qsTr("Define a payment ID for this transfer (useful for services/exchanges)")
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

            text: qsTr("Paste")
            onClicked: { inputPaymentId.text = ""; inputPaymentId.paste() }
        }

        TextField {
            id: inputPaymentId

            anchors.left: pastePaymentIDButton.right
            anchors.right: parent.right

            placeholderText: qsTr("Payment ID")

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
    property int defaultMixinCount: 0

    Button {
        id: buttonSend

        anchors.left: inputAmount.right
        anchors.top: inputAmount.top
        anchors.leftMargin: 3

//        text: "Send"
        enabled: inputAmount.acceptableInput && inputAddress.acceptableInput && !amountUsesComma

        onClicked:  if (!inputAmount.acceptableInput && parseFloat(inputAmount) !== 0 && !amountUsesComma) {
                        lastTransferError = qsTr("Please enter an amount to send");
                    }
                    else if (!inputAddress.acceptableInput) {
                        lastTransferError = qsTr("Please enter a recipient address");
                    }
                    else if (inputPaymentId.text.length != 0 && !inputPaymentId.acceptableInput) {
                        lastTransferError = qsTr("Please enter a valid payment ID (64 chars) or disable it");
                    }
                    else {

                        /* Critical part (type conversion, very much if, wow) */
                        var address = inputAddress.text

                        var text_amount = inputAmount.text
                        var parsed_amount = parseFloat(inputAmount.text)
                        var amount_mini = Math.pow(10,12) * parsed_amount

                        var text_fees = customFeesInput.text
                        var parsed_fees = parseFloat(customFeesInput.text)
                        var fees_mini = Math.pow(10,12) * parsed_fees
                        var fees_to_apply = (mainWindow.advancedInterface && customFeesCheckbox.checked && customFeesInput.acceptableInput) ? fees_mini : defaultFee

                        var payment_id = definePaymentIdCheckbox ? inputPaymentId.text : ""

                        var mixin_count = (mainWindow.advancedInterface && customMixinCountCheckbox.checked) ? customMixinCountSlider.value : defaultMixinCount;


                        console.log("Transfer")
                        console.log("Address : " + address);
                        console.log("Amount :" + amount_mini + "(" + parsed_amount + ")")
                        console.log("Fees : " + fees_mini + "("+parsed_fees+") --> " + fees_to_apply)
                        console.log("Payment ID : " + payment_id);
                        console.log("Mixin count : " + mixin_count);



                        var transferCallback = function() {
                            performTransfer(amount_mini, address, fees_to_apply, payment_id, mixin_count);
                        }

                        if (askConfirmation) {
                            transferConfirmDialog.setCallback(transferCallback);
                            transferConfirmDialog.open()
                        }
                        else {
                            transferCallback();
                        }

                    }

        style: ButtonStyle {

            label: Label {
                font.pixelSize: 18
                text: qsTr("SEND", "transfer 'send' button")
            }

        }

    }

    ConfirmTransferDialog {
        id: transferConfirmDialog

        width: 100
        height: 100

        address: inputAddress.text
        amount: parseFloat(inputAmount.text)
        fee: customFeesCheckbox.checked ? parseFloat(customFeesInput.text) : (Math.pow(10,-12)*defaultFee)
        payment_id: inputPaymentId.text

    }

    function performTransfer(amount_mini, address, fees_to_apply, payment_id, mixin_count) {

        console.log("Performing transfer...")
        var res = wallet.transfer(amount_mini, address, fees_to_apply, payment_id, mixin_count);
        lastTransactionLayout.visible = false;
    }

    Label {
        anchors.left: buttonSend.right
        anchors.leftMargin: 10
        anchors.verticalCenter: buttonSend.verticalCenter

        text: ( inputAmount.text.length > 0 ? parseFloat(inputAmount.text) + " XMR" : "" ) + ( amountUsesComma ? qsTr(" PLEASE USE '.' (dot) instead of ',' (comma) for decimals") : "")
        color: amountUsesComma ? "red" : "green"
    }

    ColumnLayout {
        id: customFeeLayout

        visible: mainWindow.advancedInterface

        anchors.top: buttonSend.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.right: parent.right

        CheckBox {
            id: customFeesCheckbox

            text: qsTr("Custom network fee", "custom fee checkbox")
        }

        RowLayout {
            visible: customFeesCheckbox.checked

            anchors.left: parent.left
            anchors.right: parent.horizontalCenter

            Label {
                id: customFeesLabel

                text: qsTr("Fee", "custom fee label") + " : "
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
        id: customMixinCountLayout

        visible: mainWindow.advancedInterface

        anchors.top: customFeeLayout.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.right: parent.right

        CheckBox {
            id: customMixinCountCheckbox

            text: qsTr("Custom mixin count")
        }

        RowLayout {
            visible: customMixinCountCheckbox.checked

            anchors.left: parent.left
            anchors.right: parent.horizontalCenter

            Label {
                id: customMixinCountLabel

                text: qsTr("Mixin count", "custom mixin count label") + " : "
            }

            Slider {
                id: customMixinCountSlider

                anchors.left: customMixinCountLabel.right
                anchors.leftMargin: 3

                anchors.right: parent.horizontalCenter

                stepSize: 1
                minimumValue: 0
                maximumValue: 5

            }

            Label {
                text: customMixinCountSlider.value

                anchors.left: customMixinCountSlider.right
                anchors.leftMargin: 3
            }


        }
    }

    ColumnLayout {
        id: lastTransactionLayout
        visible: false;

        Label {
            text: qsTr("Transaction successful") + " : "
            color: "green"
        }

        RowLayout {

            Button {
                text: qsTr("Copy Hash")
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
