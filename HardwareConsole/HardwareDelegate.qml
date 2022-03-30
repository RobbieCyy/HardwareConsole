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

    signal setHardware(int index)

    property bool chosen : ListView.view.currentIndex === index

    Component.onCompleted: {
        this.setHardware.connect(hardwareSpecificsListView.setHardwareSpecificModelByIndex)
    }

    background: Item {
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: Material.background
            anchors.bottomMargin: -20
            anchors.leftMargin: -2
            anchors.rightMargin: -2
            border.color: chosen ? "#00BCD4" : "ghostwhite"
            border.width: 2
            radius: 10
        }

        Rectangle {
            width: parent.width
            height: 20
            color: Material.background
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -20
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    onClicked: {
        ListView.view.currentIndex = index
        setHardware(index)
    }
    font.pixelSize: hovered ? 26 : 24
    font.family: "Calibri"
    font.bold: chosen
    text: chosen ? model.name.toUpperCase() : model.name
    Material.foreground: chosen ? Material.Cyan : Material.White
}
