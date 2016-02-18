import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2
import "qrc:Constant.js" as Constant

Item {
    id: item1
    width: Constant.window_width
    height: Constant.window_height

    property alias buttonPlay: buttonPlay
    property alias buttonPause: buttonPause
    property alias buttonFileSelector: buttonFileSelector
    property alias barArea: barGraphArea1
    property alias progressBar: progressBar1
    property alias elapsedTime: text1
    property alias buttonPre: buttonPre
    property alias buttonNext: buttonNext
    property alias stackView: stackView

    Rectangle {
        id : control_view
        anchors.bottom: parent.bottom
        width: Constant.control_width
        height:Constant.control_height + Constant.control_margin * 2
        color: Constant.control_bgcolor

        RowLayout {
            id : control_layout
            anchors.centerIn: parent
            anchors.bottomMargin: Constant.control_margin
            anchors.topMargin: Constant.control_margin

            Button {
                id: buttonPlay
                iconName: qsTr("Play")
                iconSource: "qrc:/../image/Play.png"
            }

            Button {
                id: buttonPause
                iconName: qsTr("Pause")
                iconSource: "qrc:/../image/Pause.png"
                enabled: false
                visible: false
            }

            Button {
                id: buttonFileSelector
                iconName: qsTr("File")
                iconSource: "qrc:/../image/Plus.png"
            }

            ProgressBar {
                id: progressBar1
                style: ProgressBarStyle {
                    background: Rectangle {
                        radius: 5
                        color: Constant.process_background_color
                        border.color: Constant.process_background_border_color
                        border.width: 1
                        implicitWidth: Constant.process_bar_width
                        implicitHeight: Constant.process_bar_heigh
                    }
                    progress: Rectangle {
                        color: Constant.process_bar_color
                        border.color: Constant.process_bar_border_colr
                    }
                }
            }

            Text {
                id: text1
                text: qsTr("00:00 / 00:00")
                color: "#00FF00"
                font.pixelSize: Constant.text_size
            }

            Text {
                id: text2
                //                text: qsTr("Effect:")
                color : "white"
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

        EffectChooseForm {
            id: effect_choose_form
            property bool isVisible: false
            x : (isVisible === false) ?  Constant.visulation_width
                                      : Constant.visulation_width - Constant.effect_choose_form_width
            Behavior on x {
                SmoothedAnimation { velocity: 200 }
            }
        }

        Timer{
            id:effect_choose_timer
            running: false
            interval: Constant.effect_choose_form_time_interval;
            onTriggered: {
                // close the effect choose form
                effect_choose_form.isVisible = false;
            }
        }


        MouseArea {
            id : effect_choose_form_mousearea
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: effect_choose_form.width
            onClicked: {
                if(effect_choose_form.isVisible === false)
                {
                    // show the effect choose form
                    effect_choose_form.isVisible = true
                    effect_choose_timer.start();
                }
                else
                {
                    // caculate the right effect choosed
                    //                    console.log("mouse.x = " + mouse.x + " mouse.y = " + mouse.y);
                    var form = effect_choose_form.getMouseEventEffect(mouse);
                    if(form != -1)
                    {
                        // jump to the correct effect view
                        mainForm.handleEffectChoosing(form.effect_index);
                    }
                }
            }
            onPressed: {
                var form = effect_choose_form.getMouseEventEffect(mouse);
                if(form != -1)
                {
                    form.border.color = Constant.effect_choose_form_border_pressed_color;
                }

            }
            onReleased: {
                // restart the timer
                effect_choose_timer.restart();

                var form = effect_choose_form.getMouseEventEffect(mouse);
                if(form != -1)
                {
                    form.border.color =  Constant.effect_choose_form_border_release_color;
                }

            }
        }

    }

}


