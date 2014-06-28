import QtQuick 2.0
import QtQuick.Dialogs 1.1

MessageDialog {


    property double amount;
    property double fee;
    property string address;
    property string payment_id;

    property var callback;

    title: qsTr("Confirm transaction")

    text: qsTr("You are about to send %L1 XMR to").arg(amount) + "\n" + address+"." + "\n\n" + qsTr("Ok to confirm transfer.")
    detailedText: qsTr("Network fee : %L1 XMR").arg(fee)

    standardButtons: StandardButton.Abort | StandardButton.Ok

    onAccepted: callback()

    function setCallback(cb) {
        callback = cb;
    }


}
