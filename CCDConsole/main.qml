import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

import AndorCCD.module 1.0

Window {
    visible: true
    width: 960
    height: 640
    title: qsTr("CCD Console")

    Timer {
        interval: 20
        running: true
        repeat: true
        property int counter: 0
        onTriggered: {
            if (ccd.keepAcquisition()) {
                ccd.startAcquisitionAndGetData()
            }
            else if(counter == 0) {
                ccd.getTemperature()
            }
            counter = (counter + 1) % 20
        }
    }

    Row {
        Image {
            id: ccdImage
            function reload() {
                source = "image://ccd/random?id=" + Math.random()
            }
            source: "image://ccd/random"
            cache: false
            asynchronous: false
        }

        Column {
            Row {
                RadioButton {
                    checked: true
                    text: qsTr("Cooler ON")
                    onClicked: ccd.coolerOn()
                }
                RadioButton {
                    text: qsTr("Cooler OFF")
                    onClicked: ccd.coolerOff()
                }
                Component.onCompleted: ccd.coolerOn()
            }

            Row {
                RadioButton {
                    text: qsTr("Open Shutter")
                    onClicked: ccd.openShutter()
                }
                RadioButton {
                    checked: true
                    text: qsTr("Close Shutter")
                    onClicked: ccd.closeShutter()
                }
                Component.onCompleted: ccd.closeShutter()
            }

            Label {
                id: temperatureLabel
                text: qsTr("Unknown temperature")
            }

            Row {
                Label {
                    text: qsTr("Desired temperature: ")
                }
                TextField {
                    text: qsTr("-80")
                    validator: IntValidator {bottom: ccd.minTemp(); top: ccd.maxTemp();}
                    focus: true
                    onAccepted: ccd.setTemperature(Number(text))
                }
                Component.onCompleted: ccd.setTemperature(-80)
            }

            Button {
                text: "Default Setting"
                onClicked: {
                    ccd.setAcquisitionMode(1) // single scan
                    ccd.setExposureTime(0.01)
                    ccd.setReadMode(4) // image
                    ccd.setImage(1, 1, 1, ccd.detectorWidth(), 1, ccd.detectorHeight()) //near “Full Resolution Image”
                    ccd.setTriggerMode(0) // internal
                    ccd.setEMCCDGain(10)
                }
            }

            Button {
                text: "Start Acquisition"
                onClicked: {
                    ccd.setDataProcessed()
                    ccd.startAcquisitionAndGetData()
                }
            }

            Button {
                text: "Stop Acquisition"
                onClicked: ccd.stopAcquisition()
            }
        }
    }

    AndorCCD {
        id: ccd

        onTemperatureChange: {
            temperatureLabel.text = qsTr("Current temperature: %1").arg(temperature.toFixed(2))
            //console.log(temperature, status)
        }
        onDataProcessed: ccdImage.reload()
        Component.onCompleted: {
            dataHandler.setCCD(this)
        }
    }
}
