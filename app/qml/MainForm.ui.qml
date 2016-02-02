import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    width: 640
    height: 480

    property alias button1: buttonPlay
    property alias button2: buttonPause
    property alias button3: buttonFileSelector
    property alias barArea: barGraphArea1
    property alias progressBar: progressBar1
    property alias elapsedTime: text1

    RowLayout {
        width: 265
        height: 27
        anchors.verticalCenterOffset: 221
        anchors.horizontalCenterOffset: 0
        anchors.centerIn: parent

        Button {
            id: buttonPlay
            text: qsTr("Play")
        }

        Button {
            id: buttonPause
            text: qsTr("Pause")
        }

        Button {
            id: buttonFileSelector
            text: qsTr("File")
        }
    }

    BarGraphArea {
        id: barGraphArea1
        x: 8
        y: 8
        width: 624
        height: 404
    }

    ProgressBar {
        id: progressBar1
        x: 8
        y: 418
        width: 624
        height: 23
    }

    Text {
        id: text1
        x: 20
        y: 453
        width: 119
        height: 15
        text: qsTr("00:00 / 00:00")
        font.pixelSize: 12
    }
}

