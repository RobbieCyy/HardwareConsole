import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11

ItemDelegate {
    id: rootHardwareSpecificsDelegate
    height: chosen ? hardwareSpecificsFunctionListView.height + hardwareSpecificsNameLabel.height : hardwareSpecificsNameLabel.height
    width: hardwareSpecificsListView.width

    highlighted: false

    onClicked: ListView.view.currentIndex = index

    background: Item {
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: Material.background
            anchors.bottomMargin: -20
            anchors.leftMargin: -2
            anchors.rightMargin: -2
            border.color: Material.theme === Material.Dark ? "ghostwhite" : "midnightblue"
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

    property bool chosen: ListView.isCurrentItem
    property ListModel functions: model.functions

    Label {
        id: hardwareSpecificsNameLabel
        font.pointSize: 20
        font.family: "Arial"
        text: model.name
        height: 40
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    ListView {
        id: hardwareSpecificsFunctionListView
        visible: rootHardwareSpecificsDelegate.chosen
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: hardwareSpecificsNameLabel.height
        width: parent.width
        model: functions
        height: 0
        onModelChanged: {
            if (model !== null) {
                height = model.count * 40
            }
        }
        delegate: HardwareSpecificsFunctionDelegate {}
    }
}

