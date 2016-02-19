// constant value for project
var default_title = "Music";


// Constant Value for ui

//var window_width = 840;
//var window_height = 480;

var window_width = 2048;
var window_height = 1536;

var window_bgcolor = "#212121"


var control_width = window_width;
var control_height = window_height /20;
var control_margin = 5;
var control_bgcolor = "#454545"


var process_bar_heigh = 10;
var process_bar_width = (control_width / 4) *3 ;
var process_background_color = "#787878";
var process_background_border_color = "gray";
var process_bar_color = "#104E8B";
var process_bar_border_colr = "steelblue";



var visulation_height = window_height - control_height - (control_margin * 2);
var visulation_width = window_width;


var bar_count = 25;
var bar_spacing = 8;
var bar_width = (visulation_width - ((bar_count + 1) * bar_spacing)) / bar_count;
var bar_interal = bar_spacing + bar_width;


var effect_choose_form_width = window_width/5;
var effect_choose_form_height = visulation_height;
var effect_choose_form_border_pressed_color = "#3A5FCD";
var effect_choose_form_border_release_color = "#404040";
var effect_choose_form_border_width = 3;
var effect_choose_form_Image_scale = 0.9;

var effect_choose_form_color = "#292929"
var effect_choose_effectCount = 4;
var effect_choose_layoutSpacing = 5;
var effect_choose_layoutMargin = 5;
var effect_choose_chooseFormHeight = (effect_choose_form_height
                                - (2 * effect_choose_layoutMargin)
                                -  (effect_choose_effectCount - 1) * effect_choose_layoutSpacing) / effect_choose_effectCount;


var effect_choose_form_time_interval = 3000;

var text_size = 10;




