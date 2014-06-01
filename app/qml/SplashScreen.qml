
import QtQuick 2.0
import QtQuick.Controls 1.1


Rectangle {
    id: splashScreenContainer

    // image source is kept as an property alias, so that it can be set from outside
    property alias imageSource: splashImage.source

    // signal emits when splashscreen animation completes
    signal splashScreenCompleted()
    Image {
        id: splashImage
        source: imageSource
        anchors.fill: splashScreenContainer // do specify the size and position
    }

    MouseArea {

        onClicked: { splashScreenContainer.splashScreenCompleted()  }
        anchors.fill: parent


    }

    // simple QML animation to give a good user experience
    SequentialAnimation {
        id:splashanimation
        PauseAnimation { duration: 4200 }
        PropertyAnimation {
            target: splashImage
            duration: 700
            properties: "opacity"
            to:0
        }

        onStopped: {
            splashScreenContainer.splashScreenCompleted();
        }

    }


    Label {
        text: "Loading..."

        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    //starts the splashScreen
    Component.onCompleted: splashanimation.start()
 }
