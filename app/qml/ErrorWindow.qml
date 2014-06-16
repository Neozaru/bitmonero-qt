import QtQuick 2.0

import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

ApplicationWindow {
    id: errorWindow

    visible: true;

    width: 400
    height: 400
    property int errorCode: error_code ? error_code : 10

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
                      "Daemon error :\n Please ensure that bitmonerod.exe is located in\nthe same folder as Monero Wallet\nor add 'spawn_wallet=false' to configuration"
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
                  case 5:
                      "Wallet configuration has failed.\nPlease try again or check bitmonero-qt.conf"
                      break;
                  case 11:
                      "Daemon error : Unable to find 'bitmonerod'"
                      break;
                  case 12:
                      "Daemon error : Unable to start 'bitmonerod'\nIs your Daemon already running ?\nPlease restart Monero Wallet"
                      break;
                  case 21:
                      "Wallet error : Unable to find 'simplewallet'"
                      break;
                  case 22:
                      "Wallet error : Unable to start 'simplewallet'.\nPlease check your version or update 'simplewallet'"
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
