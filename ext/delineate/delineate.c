#include "delineate.h"

/*
 * Document-module: Delineate
 *
 * This module represents tha application namespace
 *
 */
VALUE rb_module;

VALUE rb_module_delineate() {
  return rb_module;
}

void define_ruby_module() {
  if (rb_module)
    return;

  rb_module = rb_define_module("Delineate");
}

void Init_delineate() {
  define_ruby_module();
  define_ruby_point_class();
  define_ruby_line_class();
  extend_ruby_delineate_module();
}
