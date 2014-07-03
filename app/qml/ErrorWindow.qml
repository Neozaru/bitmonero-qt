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
            text: qsTr("An error has occurred : ")
            color: "Red"
        }

        TextArea {
            id: errorTextArea

            readOnly: true
            backgroundVisible: false


            property string daemonExec: "bitmonerod"
            property string walletExec: "simplewallet"
            property string configFile: "bitmonero-qt.conf"

            text: switch(errorWindow.errorCode) {
                  case 1:
                      qsTr("Daemon error :\n Please ensure that '%1' is located in\nthe same folder as Monero Wallet\nor add '%2' to configuration").arg(daemonExec).arg("spawn_wallet=false")
                      break;
                  case 2:
                      qsTr("Failed initialize Wallet.\nPlease check presence of '%1'\nTry again if you are importing an old wallet.").arg(walletExec)
                      break;
                  case 3:
                      qsTr("Failed to get information from your Wallet.\nPlease check your password in configuration.")
                      break;
                  case 4:
                      qsTr("Failed to open your Wallet.\nThis error can happen when importing/syncing an old wallet.\nTrying twice can resolve the problem.\nThis will be fixed in next version")
                      break;
                  case 5:
                      qsTr("Wallet configuration has failed.\nPlease try again or check '%1'").arg(configFile)
                      break;
                  case 11:
                      qsTr("Daemon error : Unable to find '%1'").arg(daemonExec)
                      break;
                  case 12:
                      qsTr("Daemon error : Unable to start '%1'\nIs your Daemon already running ?\nPlease restart Monero Wallet").arg(daemonExec)
                      break;
                  case 21:
                      qsTr("Wallet error : Unable to find '%1'").arg(walletExec)
                      break;
                  case 22:
                      qsTr("Wallet error : Unable to start '%1'.\nPlease check your version or update '%2'").arg(walletExec).arg(walletExec)
                      break;
                  case 31:
                      qsTr("Unknown wallet error. Check your 'simplewallet' executable");
                      break;
                  case 33:
                      qsTr("Wallet error : Bad wallet executable or your wallet is outdated");
                      break;
                  case 34:
                      qsTr("Wallet error : Either another instance is already running or your wallet is outdated");
                      break;
                  default:
                      qsTr("Unknown error (%L1)").arg(parseInt(errorWindow.errorCode))
                  }
        }

        Button {
            text: "Copy error"
            onClicked: {
                errorTextArea.selectAll(); errorTextArea.copy(); errorTextArea.select(0,0);
            }
        }

        Label {
            text: "Error code : " + errorWindow.errorCode;
            font.pixelSize: 10
        }

        Button {
            text: qsTr("Exit")

            onClicked: Qt.quit();
        }

    }

}
