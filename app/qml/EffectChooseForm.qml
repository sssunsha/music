import QtQuick 2.0
import QtQuick.Layouts 1.2
//import "qrc:Constant.js" as Constant

Rectangle {
    id : effect_choose_form
    width: util.effect_choose_form_width
    height: util.effect_choose_form_height
    //    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    opacity: 0.8
    color: util.effect_choose_form_color

    ColumnLayout{
        anchors.fill: parent
        anchors.topMargin: util.effect_choose_layoutMargin
        anchors.bottomMargin: util.effect_choose_layoutMargin
        spacing: util.effect_choose_layoutSpacing

        Rectangle {
            id : effect1_choose_form
            property int effect_index: 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: util.effect_choose_layoutMargin
            anchors.rightMargin: util.effect_choose_layoutMargin
            width: parent.width -(2 *  util.effect_choose_layoutMargin)
            height: util.effect_choose_chooseFormHeight
            implicitHeight: util.effect_choose_chooseFormHeight
            border.color: util.effect_choose_form_border_release_color
            border.width: util.effect_choose_form_border_width
            color : util.effect_choose_form_color
            Image {
                anchors.fill: parent
                scale: util.effect_choose_form_Image_scale
                source : "qrc:/../image/effect1.png"
            }
        }
        Rectangle {
            id : effect2_choose_form
            property int effect_index: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: util.effect_choose_layoutMargin
            anchors.rightMargin: util.effect_choose_layoutMargin
            width: parent.width -(2 *  util.effect_choose_layoutMargin)
            height: util.effect_choose_chooseFormHeight
            implicitHeight: util.effect_choose_chooseFormHeight
            border.color: util.effect_choose_form_border_release_color
            border.width: util.effect_choose_form_border_width
            color : util.effect_choose_form_color
            Image {
                anchors.fill: parent
                scale: util.effect_choose_form_Image_scale
                source : "qrc:/../image/effect2.png"
            }

        }

        Rectangle {
            id : effect3_choose_form
            property int effect_index: 2
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: util.effect_choose_layoutMargin
            anchors.rightMargin: util.effect_choose_layoutMargin
            width: parent.width -(2 *  util.effect_choose_layoutMargin)
            height: util.effect_choose_chooseFormHeight
            implicitHeight: util.effect_choose_chooseFormHeight
            border.color: util.effect_choose_form_border_release_color
            border.width: util.effect_choose_form_border_width
            color : util.effect_choose_form_color
            Image {
                anchors.fill: parent
                scale: util.effect_choose_form_Image_scale
                source : "qrc:/../image/effect3.png"
            }

        }

        Rectangle {
            id : effect4_choose_form
            property int effect_index: 3
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: util.effect_choose_layoutMargin
            anchors.rightMargin: util.effect_choose_layoutMargin
            width: parent.width -(2 *  util.effect_choose_layoutMargin)
            height: util.effect_choose_chooseFormHeight
            implicitHeight: util.effect_choose_chooseFormHeight
            border.color: util.effect_choose_form_border_release_color
            border.width: util.effect_choose_form_border_width
            color : util.effect_choose_form_color
            Image {
                anchors.fill: parent
                scale: util.effect_choose_form_Image_scale
                source : "qrc:/../image/effect4.png"
            }

        }
    }

    function getMouseEventEffect(mouse){
        // check for the y from the mouse
        var m = util.effect_choose_layoutMargin;
        var s = util.effect_choose_layoutSpacing;
        var h = util.effect_choose_chooseFormHeight;
        var y = mouse.y;
        var effect1_topy = m;
        var effect1_bottomy = effect1_topy + h;
        var effect2_topy = effect1_bottomy + s;
        var effect2_bottomy = effect2_topy + h;
        var effect3_topy = effect2_bottomy + s;
        var effect3_bottomy = effect3_topy + h;
        var effect4_topy = effect3_bottomy + s;
        var effect4_bottomy = effect4_topy + h;

        if(y >= effect1_topy && y <= effect1_bottomy)
        {
            return effect1_choose_form;
        }
        else if(y >= effect2_topy && y <= effect2_bottomy)
        {
            return effect2_choose_form;
        }
        else if(y >= effect3_topy && y <= effect3_bottomy)
        {
            return effect3_choose_form;
        }
        else if(y >= effect4_topy && y <= effect4_bottomy)
        {
            return effect4_choose_form;
        }
        return -1;
    }

    Util{
        id:util
    }

}

