import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

ItemDelegate {
    id: rootHardwareDelgete
    width: parent.width
    height: 50
    checkable: true
    highlighted: ListView.isCurrentItem

    signal setHardware(int index)

    Component.onCompleted: {
        this.setHardware.connect(hardwareSpecificsListView.setHardwareSpecificModelByIndex)
    }

    onClicked: {
        ListView.view.currentIndex = index
        setHardware(index)
    }
    font.pixelSize: hovered ? 26 : 24
    font.family: "Calibri"
    font.bold: highlighted
    text: model.name
}
