import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15

import AndorCCD.module 1.0
import SocketServer.module 1.0

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
                ccd.startAcquisitionAndGetData(false)
            }
            else if(counter == 0) {
                ccd.getTemperature()
            }
            counter = (counter + 1) % 20
        }
    }

    Row {
        Column {
            Image {
                id: ccdImage
                function reload() {
                    source = "image://ccd/random?id=" + Math.random()
                }
                source: "image://ccd/random"
                cache: false
                asynchronous: false
            }

            Label {
                text: qsTr("Ion Number: %1").arg(server.ionPositions.length)
            }
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
                    //enabled: false
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

            Row {
                Label {
                    text: qsTr("EMCCD Gain: ")
                }
                TextField {
                    text: qsTr("10")
                    validator: IntValidator {bottom: 1; top: 300;}
                    focus: true
                    onAccepted: ccd.setEMCCDGain(Number(text))
                }
                Component.onCompleted: ccd.setEMCCDGain(10)
            }

            Row {
                ComboBox {
                    textRole: "text"
                    valueRole: "value"
                    // When an item is selected, update the backend.
                    onActivated: console.log(currentValue)
                    // Set the initial currentIndex to the value stored in the backend.
                    Component.onCompleted: {
                        currentIndex = indexOfValue(1)
                        ccd.setAcquisitionMode(1) // single scan
                    }
                    model: [
                        { value: 1, text: qsTr("Single Scan") },
                        { value: 2, text: qsTr("Accumulate") },
                        { value: 3, text: qsTr("Kinetics") },
                        { value: 4, text: qsTr("Fast Kinetics") },
                        { value: 5, text: qsTr("Run till abort") }
                    ]
                }
            }

            Row {
                Label {
                    text: qsTr("Exposure time: ")
                }
                TextField {
                    text: qsTr("0.1")
                    validator: DoubleValidator {bottom: 0;}
                    focus: true
                    onAccepted: ccd.setExposureTime(Number(text))
                }
                Component.onCompleted: ccd.setExposureTime(0.1)
            }

            Row {
                ComboBox {
                    textRole: "text"
                    valueRole: "value"
                    // When an item is selected, update the backend.
                    onActivated: console.log(currentValue)
                    // Set the initial currentIndex to the value stored in the backend.
                    Component.onCompleted: {
                        currentIndex = indexOfValue(4)
                        ccd.setReadMode(4) // image
                    }
                    model: [
                        { value: 0, text: qsTr("Full Vertical Binning") },
                        { value: 1, text: qsTr("Multi-Track") },
                        { value: 2, text: qsTr("Random-Track") },
                        { value: 3, text: qsTr("Single-Track") },
                        { value: 4, text: qsTr("Image") }
                    ]
                }
            }

            Column {

                function loadParameters() {
                    ccd.setImage(1, 1, ccd.hstart(), ccd.hend(), ccd.vstart(), ccd.vend())
                }

                Label {
                    text: qsTr("Start column: ")
                }
                TextField {
                    text: qsTr("1")
                    validator: IntValidator {bottom: 1; top: ccd.detectorWidth();}
                    focus: true
                    onAccepted: {
                        ccd.setHstart(Number(text))
                        parent.loadParameters()
                    }
                }
                Label {
                    text: qsTr("End column: ")
                }
                TextField {
                    text: qsTr("%1").arg(ccd.detectorWidth())
                    validator: IntValidator {bottom: 1; top: ccd.detectorWidth();}
                    focus: true
                    onAccepted: {
                        ccd.setHend(Number(text))
                        parent.loadParameters()
                    }
                }
                Label {
                    text: qsTr("Start row: ")
                }
                TextField {
                    text: qsTr("1")
                    validator: IntValidator {bottom: 1; top: ccd.detectorHeight();}
                    focus: true
                    onAccepted: {
                        ccd.setVstart(Number(text))
                        parent.loadParameters()
                    }
                }
                Label {
                    text: qsTr("End row: ")
                }
                TextField {
                    text: qsTr("%1").arg(ccd.detectorHeight())
                    validator: IntValidator {bottom: 1; top: ccd.detectorHeight();}
                    focus: true
                    onAccepted: {
                        ccd.setVend(Number(text))
                        parent.loadParameters()
                    }
                }
                Component.onCompleted: {
                    ccd.setHstart(1)
                    ccd.setHend(ccd.detectorWidth())
                    ccd.setVstart(1)
                    ccd.setVend(ccd.detectorHeight())
                    loadParameters()
                }
            }

            Row {
                ComboBox {
                    textRole: "text"
                    valueRole: "value"
                    // When an item is selected, update the backend.
                    onActivated: console.log(currentValue)
                    // Set the initial currentIndex to the value stored in the backend.
                    Component.onCompleted: {
                        currentIndex = indexOfValue(0)
                        ccd.setTriggerMode(0) // internal
                    }
                    model: [
                        { value: 0, text: qsTr("Internal") },
                        { value: 1, text: qsTr("External") },
                        { value: 6, text: qsTr("External Start") },
                        { value: 7, text: qsTr("External Exposure") },
                        { value: 9, text: qsTr("External FVB EM") },
                        { value: 10, text: qsTr("Software Trigger") }
                    ]
                }
            }

            Row {
                Button {
                    text: "Start Acquisition"
                    onClicked: {
                        ccd.setDataProcessed()
                        ccd.startAcquisitionAndGetData(false)
                    }
                }

                Button {
                    text: "Stop Acquisition"
                    onClicked: ccd.stopAcquisition()
                }
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

    SocketServer {
        id: server

        property var ionPositions: []

        onGetImageData: {
            ccd.setDataProcessed()
            ccd.startAcquisitionAndGetData(true)
        }
        onGetImageSize: {
            let size = {}
            size.width = ccd.hend()-ccd.hstart()+1
            size.height = ccd.vend()-ccd.vstart()+1
            sendText(JSON.stringify(size))
        }
        onGetIonPositions: {
            sendText(JSON.stringify(this.ionPositions))
        }
        onSetIonPositions: {
            this.ionPositions = JSON.parse(posStr)
            console.log(this.ionPositions)
        }
        Component.onCompleted: {
            dataHandler.setServer(this)
        }
    }
}
