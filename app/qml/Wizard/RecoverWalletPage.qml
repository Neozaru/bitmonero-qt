import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "qrc:/qml/Utils"

AbstractPage {

    signal walletSeedSet(var seed)

    ColumnLayout {

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
//        anchors.fill: parent
        anchors.topMargin: 45

        Label {
            text: "Write or paste your seed in the field above.\nYour seed should be a set of space separated shorts works"
        }

        Button {

            text: "Paste seed"
            onClicked: {
                seedTextarea.text = "";
                seedTextarea.paste();
            }
        }

        TextArea {
            id: seedTextarea

            anchors.left: parent.left
            anchors.right: parent.right
        }


        Button {
            text: "Recover"
            enabled: seedTextarea.text.length > 0

            onClicked: {

                if (seedTextarea.text.length > 0) {

                   walletSeedSet(seedTextarea.text);
                   goToNext();

                }

            }
        }
    }


}
