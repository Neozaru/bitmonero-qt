import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


RowLayout {

    anchors.fill: parent

    property string lastError: ""
    ColumnLayout {

        TextField {
            id: inputAddress

            placeholderText: "Recipient address"
            maximumLength: 95
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

            onClicked:  if (!inputAmount.acceptableInput) {
                            lastError = "Please enter an amount to send";
                        }
                        else if (!inputAddress.acceptableInput || inputAddress.text.length != 95) {
                            lastError = "Please enter a recipient address";
                        }
                        else {
                            console.log("OK")
                            var res = wallet.transfer(inputAmount.text, inputAddress.text);
                            console.log(res);
                        }

        }

        Text {
            id: errorMessage
            color: "red"
            text: lastError
        }


    }



}
