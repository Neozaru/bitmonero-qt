import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

TextField {

    maximumLength: 95
    textColor: if (acceptableInput) { "green" } else { "red" }
    validator: RegExpValidator { regExp: /[a-zA-Z0-9]{95}/ }
}
