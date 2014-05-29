import QtQuick 2.0
import QtQuick.Controls 1.1

TextField {
    textColor: if (acceptableInput) { "green" } else { "red" }
    validator:RegExpValidator { regExp: /.{4,}/ }

    echoMode: 2
}
