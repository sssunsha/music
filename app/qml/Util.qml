import QtQuick 2.5

Item {    

    // constant value for project
    property string default_title : "Tieto Music"


    // Constant Value for ui

    property int window_width: 840
    property int window_height: 480
    property string window_bgcolor : "#212121"

    // for Control view

    property int control_width: window_width
    property int control_height: window_height /20
    property int control_margin: 5
    property string control_bgcolor : "#454545"

    // for process bar

    property int process_bar_heigh: 10
    property int process_bar_width: (control_width / 4) *3
    property string process_background_color : "#787878"
    property string process_background_border_color : "gray"
    property string process_bar_color : "#104E8B"
    property string process_bar_border_color : "steelblue"

    // for visualtion view

    property int visulation_height: window_height - control_height - (control_margin * 16)
    property int visulation_width: window_width

    // for visualtion view bar
    property int bar_count: 25
    property int bar_spacing: 8
    property int bar_width: (visulation_width - ((bar_count + 1) * bar_spacing)) / bar_count
    property int bar_interal: bar_spacing + bar_width

    // for effect choose view

    property int effect_choose_form_width: window_width/5
    property int effect_choose_form_height: visulation_height
    property string effect_choose_form_border_pressed_color: "#3A5FCD"
    property string effect_choose_form_border_release_color: "#404040"
    property int effect_choose_form_border_width: 3
    property real effect_choose_form_Image_scale: 0.9

    property string effect_choose_form_color: "#292929"
    property int effect_choose_effectCount: 4
    property int effect_choose_layoutSpacing: 5
    property int effect_choose_layoutMargin: 5
    property int effect_choose_chooseFormHeight: (effect_choose_form_height
                                                  - (2 * effect_choose_layoutMargin)
                                                  -  (effect_choose_effectCount - 1) * effect_choose_layoutSpacing) / effect_choose_effectCount

    property int effect_choose_form_time_interval: 3000

    // for others
    property int text_size: 10
    property int life_span_effect1: 900
    property int life_span_effect2: 2000

}

