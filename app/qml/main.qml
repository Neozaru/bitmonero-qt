import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

//include("./AboutDialog")
//import "Overview.qml"

//import "AboutDialog"


ApplicationWindow {
    visible: true
    title: "Monero Wallet"



    width: 750
    height: 480




//    Action {
//        id: openAction
//        text: "&Open"
//        shortcut: StandardKey.Open
////        iconSource: "images/document-open.png"
//        onTriggered: fileDialog.open()
//        tooltip: "Open an image"
//    }


    AboutDialog { id: aboutDialog }

    Action {
        id: aboutAction
        text: "About"
        onTriggered: aboutDialog.open()
    }



    Menu {
        id: editmenu
//        MenuItem { action: cutAction }
//        MenuItem { action: copyAction }
//        MenuItem { action: pasteAction }
        MenuSeparator {}
        Menu {
            title: "Text &Format"
//            MenuItem { action: a1 }
        }

    }

    toolBar: ToolBar {
        id: toolbar
        RowLayout {
            id: toolbarLayout
            spacing: 0
            width: parent.width
            ToolButton {
//                iconSource: "images/window-new.png"
                onClicked: window1.visible = !window1.visible
                Accessible.name: "New window"
                tooltip: "Toggle visibility of the second window"
            }
//            ToolButton { action: openAction }
            ToolButton {
//                iconSource: "images/document-save-as.png"
                tooltip: "(Pretend to) Save as..."
            }
            Item { Layout.fillWidth: true }
            CheckBox {
                id: advancedInterfaceCheckbox
                text: "Advanced Interface"
                checked: false
                enabled: false
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
//            MenuItem { action: openAction }
            MenuItem {
                text: "Close"
                shortcut: StandardKey.Quit
                onTriggered: Qt.quit()
            }
        }
        Menu {
            title: "&Edit"
//            MenuItem { action: cutAction }
//            MenuItem { action: copyAction }
//            MenuItem { action: pasteAction }
            MenuSeparator { }
            MenuItem {
                text: "Do Nothing"
                shortcut: "Ctrl+E,Shift+Ctrl+X"
                enabled: false
            }
            MenuItem {
                text: "Not Even There"
                shortcut: "Ctrl+E,Shift+Ctrl+Y"
                visible: false
            }
            Menu {
                title: "Me Neither"
                visible: false
            }
        }
        Menu {
            title: "&Help"
            MenuItem { action: aboutAction }
        }
    }

    SystemPalette {id: syspal}
    color: syspal.window
    ListModel {
        id: choices
        ListElement { text: "Banana" }
        ListElement { text: "Orange" }
        ListElement { text: "Apple" }
        ListElement { text: "Coconut" }
    }

    TabView {
        id:frame

        anchors.fill: parent
        anchors.margins: Qt.platform.os === "osx" ? 12 : 2

        Tab {

            id: controlPage
            title: "Overview"
            Overview {}

        }
        Tab {
            title: "Transfer"
            Transfer {}
        }

        Tab {
            title: "History"
            History {}
        }


        Tab {
            title: "Mining"
            Mining {}
        }

        Tab {
            title: "Debug"
        }

    }
}
