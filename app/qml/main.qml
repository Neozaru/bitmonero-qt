import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.1

import "Wizard"
import "MoneroGUI"

//Item {

    ApplicationWindow {
        id: startupWindow

        visible: false

        width: 640
        height: 480



//        SplashScreen {

//            anchors.fill: parent
//            visible: false

//            onSplashScreenCompleted: { mainWindow.visible = true; startupWindow.visible = false }
//            imageSource: "Baguette.png"

//        }


        MainWindow {
            id: mainWindow
            visible: false
        }

        WalletWizardWindow {
            visible: true;

            width: 400
            height: 300


            x: Screen.desktopAvailableWidth / 2
            y: Screen.desktopAvailableHeight / 2


            onWizardSuccess: {
                visible = false;
                mainWindow.visible = true;
            }

        }

    }
