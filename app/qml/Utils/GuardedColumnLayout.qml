import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1


/* Layout with optional error message */
ColumnLayout {

    property string errorMessage: ""

    Label {
        id: errorMessageLabel

        visible: parent.errorMessage.length > 0
        text: parent.errorMessage
        color: "red"
    }

}
