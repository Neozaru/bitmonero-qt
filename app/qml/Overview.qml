import QtQuick 2.0
import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

//import overview

Rectangle {


    anchors.fill: parent

    RowLayout {

        Layout.minimumWidth: implicitWidth
        Layout.fillWidth: false


        ColumnLayout {

            Layout.minimumWidth: implicitWidth
            Layout.fillWidth: false

            Label {
                text: "Balance : "
            }

            Label {
                id: balanceValue
                text: (wallet.balance * Math.pow(10,-12)).toFixed(12)
            }



        }

    }


}
