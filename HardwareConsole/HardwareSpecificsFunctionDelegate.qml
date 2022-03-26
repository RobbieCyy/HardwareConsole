import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

ItemDelegate {
    id: rootHardwareSpecificsFunctionDelegate
    height: 40
    width: parent.width

    onClicked: {
        ListView.view.currentIndex = index
    }
    font.pixelSize: 18
    font.family: "Arial"
    text: model.name
}
