
import QtQuick 2.2
import QtQuick.Controls 1.1

import QtQuick.Window 2.1
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: splashScreenContainer
    visible: true

    title: qsTr("Monero Wallet", "window title")
    flags: Qt.FramelessWindowHint

    property int fixedWidth: 720;
    property int fixedHeight: 500;

    property bool complete: false;

    width: fixedWidth
    height: fixedHeight

    maximumHeight : fixedHeight
    maximumWidth : fixedWidth
    minimumHeight : fixedHeight
    minimumWidth : fixedWidth

    x: (Screen.width - fixedWidth) / 2
    y: (Screen.height - fixedHeight) / 2

    Rectangle {

        anchors.fill: parent;

        border.width: 2
        border.color: "black"
        smooth: true


        // signal emits when splashscreen animation completes
        signal splashScreenCompleted()
        Image {
            id: splashImage

            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            fillMode: Image.PreserveAspectFit
            smooth: true

            source: "res/MoneroLogo.png"

        }


        SequentialAnimation {
            id:splashanimation
            PauseAnimation { duration: 13000 }
            PropertyAnimation {
                target: splashCanTakeLongLabel
                duration: 2000
                properties: "opacity"
                to:100
            }

            onStopped: {

            }

        }


        RowLayout {

            anchors.centerIn: parent

            BusyIndicator {
                id: splashProgressbar
                anchors.centerIn: parent

                running: true
            }


            Label {
                id: splashPleaseWait

                anchors.top: splashProgressbar.bottom
                anchors.horizontalCenter: splashProgressbar.horizontalCenter

                anchors.margins: 10

                text: qsTr("Monero Wallet is loading. Please wait.")

                font.pixelSize: 16

            }

            Label {
                id: splashCanTakeLongLabel

                anchors.top: splashPleaseWait.bottom
                anchors.horizontalCenter: splashPleaseWait.horizontalCenter

                anchors.margins: 50
                opacity: 0

                text: qsTr("This operation can take minutes.")

                font.pixelSize: 14


            }
        }


        Label {
            text: qsTr("Loading Monero Core...", "loading details")

            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 3
            anchors.bottomMargin: 3
        }

        Connections {
            target: wallet;
            onReadyChanged: {
                console.log("READY : " + ready);
                if ( ready) {
                    splashanimation.start()
                }
            }
        }


        //starts the splashScreen
        Component.onCompleted: splashanimation.start()
    }

 }
