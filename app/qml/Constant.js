
var window_width = 840;
var window_height = 480;

var control_width = window_width;
var control_height = window_height /20;
var control_margin = 5;


var visulation_height = window_height - control_height - (control_margin * 2);
var visulation_width = window_width;


var bar_count = 25;
var bar_spacing = 8;
var bar_width = (visulation_width - ((bar_count + 1) * bar_spacing)) / bar_count;
var bar_interal = bar_spacing + bar_width;

var text_size = 10;

