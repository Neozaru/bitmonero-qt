import QtQuick 2.2
import QtQuick.Dialogs 1.1


MessageDialog {
    icon: StandardIcon.Information
    title: "Transfer successful"
    text: "The transfer was successful"
    detailedText: "Tx hash :"
    wallet.onTransferSuccessful: visible = true
}
