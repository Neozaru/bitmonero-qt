import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


GuardedColumnLayout {
    id: transferLayout

    property string lastTransferError: ""

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

    TextField {
        id: inputAmount

        anchors.top: inputAddress.bottom
        anchors.topMargin: 5

        placeholderText: "0.00"
        onTextChanged: lastTransferError = ""

        validator: DoubleValidator { bottom: 0.0; top: 100000000 }
    }

    Button {
        id: buttonSend

        anchors.left: inputAmount.right
        anchors.top: inputAmount.top
        anchors.leftMargin: 3

        text: "Send"
        enabled: inputAmount.acceptableInput && inputAddress.acceptableInput

        onClicked:  if (!inputAmount.acceptableInput) {
                        lastTransferError = "Please enter an amount to send";
                    }
                    else if (!inputAddress.acceptableInput) {
                        lastTransferError = "Please enter a recipient address";
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

            inputAmount.text = ""
            inputAddress.text = ""
            lastTransactionHash.text = tx_hash
            lastTransactionLayout.visible = true;

        }

    }

    Text {
        id: errorMessage
        color: "red"
        text: lastTransferError
    }






}
