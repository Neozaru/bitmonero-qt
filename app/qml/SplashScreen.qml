
import QtQuick 2.2
import QtQuick.Controls 1.2

import QtQuick.Window 2.1
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: splashScreenContainer
    visible: true

    title: "Monero Wallet"
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


        // image source is kept as an property alias, so that it can be set from outside
//        property alias imageSource: splashImage.source
//        imageSource: "Baguette.png"

        // signal emits when splashscreen animation completes
        signal splashScreenCompleted()
        Image {
            id: splashImage

            anchors.margins: 10
            anchors.top: parent.top
            anchors.left: parent.left

            source: "res/MoneroLogo.jpg"
//            anchors.centerIn: parent

//            anchors.fill: splashScreenContainer // do specify the size and position
        }

//        MouseArea {

//            onClicked: { splashScreenContainer.splashScreenCompleted()  }
//            anchors.centerIn: parent


//        }

//        // simple QML animation to give a good user experience
        SequentialAnimation {
            id:splashanimation
            PauseAnimation { duration: 10000 }
            PropertyAnimation {
                target: splashCanTakeLongLabel
                duration: 2000
                properties: "opacity"
                to:100
            }

            onStopped: {
//                splashScreenContainer.splashScreenCompleted();
            }

        }


        RowLayout {

            anchors.centerIn: parent


            ProgressBar {
               id: splashProgressbar

               anchors.centerIn: parent
               indeterminate: true
            }

            Label {
                id: splashPleaseWait

                anchors.top: splashProgressbar.bottom
                anchors.horizontalCenter: splashProgressbar.horizontalCenter

                anchors.margins: 10

                text: "Monero Wallet is loading. Please wait."


            }

            Label {
                id: splashCanTakeLongLabel

                anchors.top: splashPleaseWait.bottom
                anchors.horizontalCenter: splashPleaseWait.horizontalCenter

                anchors.margins: 50
                opacity: 0

                text: "This operation can take minutes if you are importing an old wallet."


            }
        }



        Label {
            text: "Refreshing wallet..."

            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }

        Connections {
            target: wallet;
            onReadyChanged: {
                console.log("CCCCCCCCCCCCCCCCCCCCCCC");
                console.log(ready);
                if ( ready) {
                    splashanimation.start()
                }
            }
        }



        //starts the splashScreen
        Component.onCompleted: splashanimation.start()
    }

 }
