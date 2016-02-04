import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    id: item1
    width: 840
    height: 480

    property alias button1: buttonPlay
    property alias button2: buttonPause
    property alias button3: buttonFileSelector
    property alias barArea: barGraphArea1
    property alias progressBar: progressBar1
    property alias elapsedTime: text1
    property alias buttonPre: buttonPre
    property alias buttonNext: buttonNext
    property alias stackView: stackView

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
    StackView {
        id: stackView
        x: 8
        y: 8
        width: 824
        height: 404
        Rectangle {

            width: 824
            height: 404
            BarGraphArea {
                id: barGraphArea1

                width: 824
                height: 404
            }
        }
    }

    ProgressBar {
        id: progressBar1
        x: 8
        y: 418
        width: 824
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

    RowLayout {
        id: rowLayout1
        x: 629
        y: 447
        width: 220
        height: 25
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8

        Text {
            id: text2
            text: qsTr("Effect:")
            font.pixelSize: 12
        }

        Button {
            id: buttonPre
            text: qsTr("<-")
        }

        Button {
            id: buttonNext
            text: qsTr("->")
        }


    }
}

