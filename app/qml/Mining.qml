import QtQuick 2.2
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


ColumnLayout {

    anchors.fill: parent;
    anchors.margins: 10

    RowLayout {
        id: statusLayout;
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            id: statusTitle


            anchors.right: statusLabel.left

            text: "Current status : "
        }

        Label {
            id: statusLabel

            anchors.right: parent.right

            text: switch(miner.status) {
                  case 0:
                      "Disabled"
                      break;
                  case 1:
                      "Mining"
                  }

            color: switch(miner.status) {
                   case 0:
                       "red"
                       break;
                   case 1:
                       "green"
                }
        }
    }

    RowLayout {
        id: toggleMiningLayout

        anchors.top: statusLayout.bottom
        anchors.topMargin: 15;

        CheckBox {
            id: toggleMiningSwitch
            text: "Enable Mining"
            enabled: miningAddressInput.acceptableInput && miningNbThreadsInput.acceptableInput
            checked: miner.enabled
            onCheckedChanged: miner.enabled = checked
//            onCheckedChanged: toggleMiningLayout.eee = checked
        }

        /* Two-ways binding not available in Qt Quick ? oO */
        Connections {
            target: miner;
            onEnabledChanged: {
                toggleMiningSwitch.checked = miner.enabled
            }
        }


    }

    ColumnLayout {

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.top: toggleMiningLayout.bottom
        anchors.topMargin: 10

        enabled: !toggleMiningSwitch.checked

        Label {
            text: "Number of threads"
        }

        TextField {
            id: miningNbThreadsInput;
            text: "1"
            validator: IntValidator { bottom: 1; top: 4; }
            onTextChanged: if ( miningNbThreadsInput.acceptableInput ) { miner.nbThreads = text; console.log(text) }
        }

        CheckBox {
            id: useAnotherAddressCheckbox
            text: "Enter mining address manually"
            checked: miner.address.length == 0
        }

        RowLayout {

            anchors.left: parent.left
            anchors.right: parent.right

            visible: useAnotherAddressCheckbox.checked
            Button {
                id: pasteMiningAddressButton
                text: "Paste"
                onClicked: { miningAddressInput.text = ""; miningAddressInput.paste() }
            }

            AddressTextField {
                anchors.left: pasteMiningAddressButton.right
                anchors.right: parent.right
                id: miningAddressInput
                placeholderText: "Target address for mining"
                text: miner.address
                onTextChanged: if (miningAddressInput.acceptableInput || text.length == 0) { miner.address = text }
            }


        }




//        TextField {
//            id: miningAddressInput
//            visible: useAnotherAddressCheckbox.checked



//            text: ""
//            placeholderText: "Target address for mining"

//        }
    }

    ColumnLayout {


        visible: miner.status == 1

//        Label {
//            id: statusLabel;
//            text: if ( miner.status == 1 ) { "ON" } else { "OFF" }
//        }

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
