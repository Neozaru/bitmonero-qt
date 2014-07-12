import QtQuick 2.2
import QtQuick.Controls 1.1

TextField {

    property string paymentIdSeperator: '+';

    maximumLength: 95 + paymentIdSeperator.length + 64

    textColor: acceptableInput ? "green" : "red"
    validator: RegExpValidator { regExp: /^[a-zA-Z0-9]{95}(\+[a-f0-9]{64})?$/g }


}
