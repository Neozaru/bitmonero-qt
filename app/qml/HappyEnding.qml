import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Window 2.1

ApplicationWindow {
    id: happyEndingWindow

    visible: true

    title: qsTr("Monero Wallet", "window title")
//    flags: Qt.FramelessWindowHint

    property int fixedWidth: 250;
    property int fixedHeight: 100;


    width: fixedWidth
    height: fixedHeight

    maximumHeight : fixedHeight
    maximumWidth : fixedWidth
    minimumHeight : fixedHeight
    minimumWidth : fixedWidth

    x: (Screen.width - fixedWidth) / 2
    y: (Screen.height - fixedHeight) / 2

    Rectangle {

        anchors.fill: parent

        border.width: 1
        border.color: "black"
        smooth: true

        Label {

            anchors.centerIn: parent
            text: qsTr("Closing Monero Wallet. Please wait...")
        }
    }

    Component.onCompleted: {
//        re
        happyEndingWindow.update()
        happyEndingWindow.raise()
        happyEndingWindow.setVisible(true)
    }
}
