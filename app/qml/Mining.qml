import QtQuick 2.2
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


ColumnLayout {

    anchors.fill: parent;
    anchors.margins: 10


    RowLayout {
        id: toggleMiningLayout
        anchors.top: parent.top
        anchors.left: parent.left

//        Switch {
//            id: toggleMiningSwitch
//            checked: miningEnabled
//        }

        CheckBox {
            id: toggleMiningSwitch
            text: "Enable Mining"
            checked: miner.enabled
            onCheckedChanged: miner.enabled = checked
        }

        Label {
            text: { if (toggleMiningSwitch.checked)  {"Mining Enabled"} else { "Mining Disabled" } }
        }


    }

    ColumnLayout {

        anchors.top: toggleMiningLayout.bottom
        anchors.topMargin: 10

        enabled: !toggleMiningSwitch.checked

        Label {
            text: "Number of threads"
        }

        TextField {

            text: "1"
            validator: IntValidator { bottom: 1; top: 4 }
        }

        CheckBox {
            id: useAnotherAddressCheckbox
            text: "Mine to another address"
        }


        AddressTextField {
            id: miningAddressInput
            visible: useAnotherAddressCheckbox.checked
            placeholderText: "Target address for mining"
            text: miner.address
            onTextChanged: if (miningAddressInput.acceptableInput || text.length == 0) { miner.address = text }
        }

//        TextField {
//            id: miningAddressInput
//            visible: useAnotherAddressCheckbox.checked



//            text: ""
//            placeholderText: "Target address for mining"

//        }
    }

    ColumnLayout {


        visible: toggleMiningSwitch.checked

        Label {
            id: statusLabel;
            text: if ( miner.status == 1 ) { "ON" } else { "OFF" }
        }

        Label {
            id: currentHashrateLabel;

            text: "Current hash rate : "
        }

        TextArea {

            readOnly: true;
            text: "Hello\nmultiline"

        }

    }

}
