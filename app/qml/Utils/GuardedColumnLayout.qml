import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1


/* Layout with optional error message */
ColumnLayout {

    anchors.top: parent.top
    anchors.margins: 10
//    anchors.topMargin: 5
    property string errorMessage: ""

    Label {
        id: errorMessageLabel

        anchors.top: parent.top
        anchors.topMargin: 2

        visible: parent.errorMessage.length > 0
        text: parent.errorMessage
        color: "red"
    }

}
