
import QtQuick 2.0
import QtQuick.Controls 1.1

import QtQuick.Window 2.1
import QtQuick.Layouts 1.1

ApplicationWindow {
    id: splashScreenContainer
    visible: true

    title: "Monero Wallet"

    property int fixedWidth: 720;
    property int fixedHeight: 500;

    width: fixedWidth
    height: fixedHeight

    maximumHeight : fixedHeight
    maximumWidth : fixedWidth
    minimumHeight : fixedHeight
    minimumWidth : fixedWidth


    Rectangle {

        anchors.fill: parent;

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
//        SequentialAnimation {
//            id:splashanimation
//            PauseAnimation { duration: 4200 }
//            PropertyAnimation {
//                target: splashImage
//                duration: 700
//                properties: "opacity"
//                to:0
//            }

//            onStopped: {
//                splashScreenContainer.splashScreenCompleted();
//            }

//        }


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

                anchors.top: splashPleaseWait.bottom
                anchors.horizontalCenter: splashPleaseWait.horizontalCenter

                anchors.margins: 50

                text: "This operation can take minutes if you are importing an old wallet."


            }
        }



        Label {
            text: "Refreshing wallet..."

            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }

        //starts the splashScreen
        Component.onCompleted: splashanimation.start()
    }

 }
