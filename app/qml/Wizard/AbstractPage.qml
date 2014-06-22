import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0


  ColumnLayout {

    property var stack;
    property var nextPage;


    function goToNext() {
        if (nextPage) {
            stack.push(nextPage)
        }
        else {
            console.warn("No nextPage defined");
        }

    }

    function goTo(page) {

        if(page) {
            stack.push(page)
        }
        else {
            console.warn("Page not defined " + page)
        }
    }

    Button {

        text: "<--"

        onClicked: {
            stack.pop();
        }
    }
}

