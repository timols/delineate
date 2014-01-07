#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

#include "delineate.h"

VALUE rb_line_class();

void define_ruby_line_class();

VALUE rb_line_compatible_q(VALUE klass, VALUE object);
VALUE rb_line_orientation_q(VALUE klass, VALUE object);
VALUE rb_line_allocate(VALUE klass);
VALUE rb_line_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_line_start_point(VALUE self);
VALUE rb_line_set_start_point(VALUE self, VALUE start_point);
VALUE rb_line_end_point(VALUE self);
VALUE rb_line_set_end_point(VALUE self, VALUE end_point);
VALUE rb_line_width(VALUE self);
VALUE rb_line_set_width(VALUE self, VALUE width);

VALUE rb_line_to_s(VALUE self);
VALUE rb_line_to_ary(VALUE self);

VALUE new_line_object(DELine line);

DELine* DELINE(VALUE object);
DELine VALUE_TO_DELINE(VALUE object);
#endif
