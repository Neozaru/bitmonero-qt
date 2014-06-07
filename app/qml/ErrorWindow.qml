import QtQuick 2.0

import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

ApplicationWindow {
    id: errorWindow

    visible: true;

    width: 400
    height: 400
    property int errorCode: error_code

    ColumnLayout {

        anchors.centerIn: parent

        Label {
            text: "An error has occurred : "
            color: "Red"
        }

        Label {
            id: errorLabel

            text: switch(errorWindow.errorCode) {
                  case 1:
                      "Daemon error"
                      break;
                  case 2:
                      "Failed initialize Wallet.\nPlease check presence of 'simplewallet.exe'\nTry again if you are importing an old wallet."
                      break;
                  case 3:
                      "Failed to get information from your Wallet.\nPlease check your password in configuration."
                      break;
                  case 4:
                      "Failed to open your Wallet.\nThis error can happen when importing/syncing an old wallet.\nTrying twice can resolve the problem.\nThis will be fixed in next version"
                      break;
                  default:
                      "Unknown error (" + parseInt(errorWindow.errorCode) + ")"
                  }
        }

        Button {
            text: "Exit"

            onClicked: Qt.quit();
        }

    }

}
