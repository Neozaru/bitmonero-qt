import QtQuick 2.2
import QtQuick.Controls 1.1

TextField {

    maximumLength: 95
    textColor: acceptableInput ? "green" : "red"
    validator: RegExpValidator { regExp: /[a-zA-Z0-9]{95}/ }
}
