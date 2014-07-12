import QtQuick 2.0

import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1

Window {

    width: 300
    height: 400
    modality: Qt.WindowModal

    ColumnLayout {

        anchors {
            fill: parent
            margins: 10
        }

        RowLayout {

            anchors {
                left: parent.left
                right: parent.right
            }

            Label {
                text: qsTr("Daemon uri")
            }

            TextField {
                id: moneroUriInput

                anchors.right: parent.right

                text: settings.monero_uri;
            }

        }

        RowLayout {

            anchors {
                left: parent.left
                right: parent.right
            }


            Label {
                text: qsTr("Daemon port")
            }

            TextField {
                id: moneroPortInput
                anchors.right: parent.right

                text: settings.monero_port

                validator: IntValidator { bottom: 1000; top: 65535}
            }

        }


        RowLayout {

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            Button {
                anchors.left: parent.left

                text: "Close"
                onClicked: close()

            }

            Button {
                anchors.right: parent.right

                text: "Save & Close"

                /* TODO : Refactoring */
                onClicked: {
                    console.log("Saving configuration")

                    if (moneroUriInput.text.length > 0) {
                        settings.setMoneroUri(moneroUriInput.text);
                    }
                    else {
                        moneroUriInput.text = settings.monero_uri
                    }

                    if (moneroPortInput.text.length > 0 && moneroPortInput.acceptableInput) {
                        settings.setMoneroPort(parseInt(moneroPortInput.text));
                    }
                    else {
                        moneroPortInput.text = settings.monero_port
                    }

                    settings.saveGlobalConfiguration();

                    close();
                }
            }

        }

    }
}
