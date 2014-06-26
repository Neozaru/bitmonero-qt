import QtQuick 2.0
import QtQuick.Dialogs 1.1

MessageDialog {


    property double amount;
    property double fee;
    property string address;
    property string payment_id;

    property var callback;

    title: "Confirm transaction."

    text: qsTr("You are about to send %L1 XMR to\n%2.\n\nOk to confirm transfer.").arg(amount).arg(address)
    detailedText: qsTr("Network fee : %L1 XMR").arg(fee)

    standardButtons: StandardButton.Abort | StandardButton.Ok

    onAccepted: callback()

    function setCallback(cb) {
        callback = cb;
    }


}
