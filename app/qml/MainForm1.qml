import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import "qrc:Constant.js" as Constant

Item {
    id: item1
    width: Constant.window_width
    height: Constant.window_height

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
        id : control_layout
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Constant.control_margin
        anchors.topMargin: Constant.control_margin
        width: Constant.control_width
        height:Constant.control_height

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

        ProgressBar {
            id: progressBar1
        }

        Text {
            id: text1
            text: qsTr("00:00 / 00:00")
            font.pixelSize: Constant.text_size
        }

        Text {
            id: text2
            text: qsTr("Effect:")
            font.pixelSize: Constant.text_size
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
    StackView {
        id: stackView
        anchors.top: parent.top
        width : Constant.visulation_width
        height : Constant.visulation_height

        Rectangle {
            anchors.fill: parent
            BarGraphArea {
                id: barGraphArea1
                anchors.fill: parent
            }
        }
    }
}
