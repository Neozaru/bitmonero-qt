import QtQuick 2.0

TextEdit {
    id: balanceValue

    property double balance: undefined

    readOnly: true
    text: if ( balance != -1 ) { (balance * Math.pow(10,-12)).toFixed(12) } else { "???" }
}