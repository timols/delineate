#ifndef DETECTION_H_INCLUDED
#define DETECTION_H_INCLUDED

#include "line_detection.h"

void extend_ruby_delineate_module();
VALUE rb_open_image(VALUE module, VALUE file_name);
VALUE rb_close_image(VALUE module);
VALUE rb_draw_horizontal_lines(VALUE module, VALUE file_name, VALUE min_length, VALUE skip_gap);
VALUE rb_draw_vertical_lines(VALUE module, VALUE file_name, VALUE min_length, VALUE skip_gap);
VALUE rb_horizontal_lines(VALUE module, VALUE min_length, VALUE skip_gap);
VALUE rb_vertical_lines(VALUE module, VALUE min_length, VALUE skip_gap);

#endif
