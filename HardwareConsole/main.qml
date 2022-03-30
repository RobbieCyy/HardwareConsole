import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import QtQml.Models 2.12
import QtQuick.Controls.Material 2.12
import JsonReader 1.0

// version 1.0

ApplicationWindow {
    id: mainWindow
    width: 700
    height: 420
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    visible: true
    title: qsTr("Hardware Console")
    property string settingsPath: "../user-file/"

    JsonReader {
        id: hardwareListReader
        source: settingsPath + "hardwareList.json"
    }

    Label {
        id: menuLabel
        text: qsTr("Menu")
        font.family: "Arial Black"
        font.pointSize: 30
        width: 250
        height: 50
        anchors.left: parent.left
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    ListView {
        id: hardwareListView
        width: menuLabel.width
        height: parent.height-menuLabel.height
        anchors.bottom: parent.bottom
        model: ListModel {}
        delegate: HardwareDelegate {}
        Component.onCompleted: {
            model.clear()
            var data = hardwareListReader.readJsonFile()
            //console.log(JSON.stringify(data["hardwares"]))
            for (var i in data["hardwares"]) {
                model.append({name: data["hardwares"][i]["category"]})
            }
        }
    }

    Rectangle {
        id: verticalSeperator
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: hardwareListView.width
        color: Material.theme === Material.Dark ? "white" : "black"
        height: parent.height
        width: 2
    }

    ListView {
        id: hardwareSpecificsListView
        width: parent.width - menuLabel.width - verticalSeperator.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        model: ListModel {}
        delegate: HardwareSpecificsDelegate {}
        Component.onCompleted: {
            setHardwareSpecificModelByIndex(0)
        }

        function setHardwareSpecificModelByIndex(index) {
            model.clear()
            var data = hardwareListReader.readJsonFile()["hardwares"][index]
            //console.log(index)
            //console.log(JSON.stringify(data))
            for (var i in data["specifics"]) {
                //console.log("setting specifics ", i, ", ", data["specifics"][i]["name"])
                model.append({name: data["specifics"][i]["name"], functions: data["functions"]})
            }
        }
    }
}
