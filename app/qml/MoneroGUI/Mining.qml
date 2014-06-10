import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import "qrc:/qml/Utils/"
import "Common"

GuardedColumnLayout {
    id: miningLayout

    anchors.fill: parent;

    RowLayout {
        id: toggleMiningLayout

        anchors.top: parent.top
        anchors.topMargin: 20

        anchors.bottomMargin: 15;

        Switch {
            id: toggleMiningSwitch

            enabled: miningAddressInput.acceptableInput && miningControlLayout.threads_count > 0 && miningControlLayout.threads_count <= miningControlLayout.max_threads
            checked: miner.enabled

            onCheckedChanged: miner.enabled = checked


            style: SwitchStyle {
                groove: Rectangle {
                         implicitWidth: 80
                         implicitHeight: 20
                         border.color: control.activeFocus ? "darkblue" : "gray"
                         border.width: 1
                 }
            }
        }

        Label {
            text: "Toggle mining"

            font.pixelSize: 16
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
        id: miningControlLayout

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.top: toggleMiningLayout.bottom
        anchors.topMargin: 20

        enabled: !toggleMiningSwitch.checked

        property int threads_count: 1
        property int max_threads: 32
        property int threads_treshold: 4

        Label {
            text: "Intensity (Number of threads) :"
        }

        onThreads_countChanged: {
            miner.nbThreads = miningControlLayout.threads_count;
        }

        RowLayout {
            id: threadsSliderLayout

            anchors.left: parent.left
            anchors.right: parent.right


            Slider {
                id: nbthreadsSlider
                anchors.left: parent.left
                anchors.right: parent.horizontalCenter

                minimumValue: 1
                maximumValue: miningControlLayout.max_threads
                stepSize: 1
                value: miningControlLayout.threads_count
                updateValueWhileDragging: true
                tickmarksEnabled: true

                onValueChanged: {
                    miningControlLayout.threads_count = value
                }

                style: SliderStyle {
                    groove: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 4
                        color: "steelblue"
                        radius: 8
                    }
                    handle: Rectangle {
                        anchors.centerIn: parent
                        color: control.pressed ? "white" : "lightgray"
                        border.color: "gray"
                        border.width: 2
                        width: 20
                        height: 20
                        radius: 12
                    }
                }

            }

            Label {

                anchors.left: nbthreadsSlider.right
                anchors.leftMargin: 10

                text: parseInt(miningControlLayout.threads_count)

                color: miningControlLayout.threads_count > 8 ? "red" : (miningControlLayout.threads_count > 4 ? "purple" : "black")
                font.bold: miningControlLayout.threads_count > 8

                font.pixelSize: 16
            }
        }

        Label {

            visible: miningControlLayout.threads_count > miningControlLayout.threads_treshold

            text: "Intensity is TOO HIGH. Please ensure you know what you are doing."
            color: "red"

        }

        CheckBox {
            id: useAnotherAddressCheckbox

            anchors.top: threadsSliderLayout.bottom
            anchors.topMargin: 20

            text: "Enter mining address manually"
            checked: miner.address.length == 0
        }

        RowLayout {

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: useAnotherAddressCheckbox.bottom
            anchors.topMargin: 5

            visible: useAnotherAddressCheckbox.checked
            Button {
                id: pasteMiningAddressButton
                text: "Paste"
                onClicked: { miningAddressInput.text = ""; miningAddressInput.paste() }
            }

            AddressTextField {
                id: miningAddressInput

                anchors.left: pasteMiningAddressButton.right
                anchors.right: parent.right

                placeholderText: "Target address for mining"
                text: miner.address
                onTextChanged: if (miningAddressInput.acceptableInput || text.length == 0) { miner.address = text }

            }


        }


    }

    RowLayout {

        visible: miner.status == 1

        BusyIndicator {
            running: miner.status == 1

        }

        Label {
            id: currentHashrateLabel;

            text: "Current hash rate : "

            font.pixelSize: 16
        }

        HashRate {
            hashrate: miner.hashrate

            font.pixelSize: 20
            color: "green"
        }



    }

}
