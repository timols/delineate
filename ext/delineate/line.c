#include "line.h"

/*
 * Document-class: Delineate::Line
 *
 * This class means one start point and one end point.
 *
 * C structure is here, very simple.
 *   typdef struct DELine {
 *     CVPoint2D32F start_line;
 *     CVPoint2D32F end_line;
 *     float width;
 *   }
 */
VALUE rb_line_klass;

VALUE rb_line_class() {
  return rb_line_klass;
}

void define_ruby_line_class() {
  if(rb_line_klass)
    return;
  /*
   * delineate = rb_define_module("Delineate");
   *
   * note: this comment is used by rdoc.
   */
  VALUE delineate = rb_module_delineate();

  rb_line_klass = rb_define_class_under(delineate, "Line", rb_cObject);
  rb_define_alloc_func(rb_line_klass, rb_line_allocate);
  rb_define_singleton_method(rb_line_klass, "compatible?", RUBY_METHOD_FUNC(rb_line_compatible_q), 1);
  rb_define_private_method(rb_line_klass, "initialize", RUBY_METHOD_FUNC(rb_line_initialize), -1);
  rb_define_method(rb_line_klass, "start_point", RUBY_METHOD_FUNC(rb_line_start_point), 0);
  rb_define_method(rb_line_klass, "start_point=", RUBY_METHOD_FUNC(rb_line_set_start_point), 1);
  rb_define_method(rb_line_klass, "end_point", RUBY_METHOD_FUNC(rb_line_end_point), 0);
  rb_define_method(rb_line_klass, "end_point=", RUBY_METHOD_FUNC(rb_line_set_end_point), 1);
  rb_define_method(rb_line_klass, "width", RUBY_METHOD_FUNC(rb_line_width), 0);
  rb_define_method(rb_line_klass, "width=", RUBY_METHOD_FUNC(rb_line_set_width), 1);

  rb_define_method(rb_line_klass, "to_s", RUBY_METHOD_FUNC(rb_line_to_s), 0);
  rb_define_method(rb_line_klass, "to_ary", RUBY_METHOD_FUNC(rb_line_to_ary), 0);
  rb_define_alias(rb_line_klass, "to_a", "to_ary");
}

/*
 * call-seq:
 *   combatible?(obj)
 *
 * Return compatibility to Line. Return true if object have method #start_point,
 * #end_point and #width.
 *
 */
VALUE rb_line_compatible_q(VALUE klass, VALUE object) {
  return (rb_respond_to(object, rb_intern("start_point")) && rb_respond_to(object, rb_intern("end_point")) && rb_respond_to(object, rb_intern("width"))) ? Qtrue : Qfalse;
}

VALUE rb_line_allocate(VALUE klass) {
  DELine *ptr;
  return Data_Make_Struct(klass, DELine, 0, -1, ptr);
}

/*
 * call-seq:
 *   new(obj)
 *   new(start, end, width)
 *
 * Create new line.
 *
 * new() is same as new(Point.new, Point.new, 0.0)
 *
 * new(obj) is same as new(obj.start_point, obj.end_point, obj.width)
 */
VALUE rb_line_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE obj, start, end, width;
  switch (argc) {
  case 1:
    obj = argv[0];
    if (rb_line_compatible_q(rb_line_klass, obj)) {
      DELINE(self)->start_point = VALUE_TO_CVPOINT2D32F(rb_funcall(obj, rb_intern("start_point"), 0));
      DELINE(self)->end_point = VALUE_TO_CVPOINT2D32F(rb_funcall(obj, rb_intern("end_point"), 0));
      DELINE(self)->width = NUM2DBL(rb_funcall(obj, rb_intern("width"), 0));
    }
    else {
      rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(rb_line_klass));
    }
    break;
  case 3:
    start = argv[0], end = argv[1], width = argv[2];
    DELINE(self)->start_point = VALUE_TO_CVPOINT2D32F(start);
    DELINE(self)->end_point = VALUE_TO_CVPOINT2D32F(end);
    DELINE(self)->width = NUM2DBL(width);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 1..3)", argc);
  }
  return Qnil;
}

/*
 * Return start point.
 */
VALUE rb_line_start_point(VALUE self) {
  return new_point_object(DELINE(self)->start_point);
}

/*
 * call-seq:
 *   start_point = val
 *
 * Set start_point parameter, return self.
 */
VALUE rb_line_set_start_point(VALUE self, VALUE start_point) {
  DELINE(self)->start_point = VALUE_TO_CVPOINT2D32F(start_point);
  return self;
}

/*
 * Return end point.
 */
VALUE rb_line_end_point(VALUE self) {
  return new_point_object(DELINE(self)->end_point);
}

/*
 * call-seq:
 *   end_point = val
 *
 * Set end_point, return self.
 */
VALUE rb_line_set_end_point(VALUE self, VALUE end_point) {
  DELINE(self)->end_point = VALUE_TO_CVPOINT2D32F(end_point);
  return self;
}

/*
 * Return width
 */
VALUE rb_line_width(VALUE self) {
  return rb_float_new(DELINE(self)->width);
}

/*
 * call-seq:
 *   width = val
 *
 * Set width, return self.
 */
VALUE rb_line_set_width(VALUE self, VALUE width) {
  DELINE(self)->width = NUM2DBL(width);
  return self;
}

/*
 * call-seq:
 *   to_s -> "<Delineate::Line: width=(self.width) start_point=[] end_point=[]>"
 *
 * Return width.
 */
VALUE rb_line_to_s(VALUE self) {
  const int i = 7;
  VALUE str[i];
  str[0] = rb_str_new2("<%s: width=(%.1f) start_point=[%.1f, %.1f] end_point=[%.1f, %.1f]>");
  str[1] = rb_str_new2(rb_class2name(CLASS_OF(self)));
  str[2] = rb_line_width(self);
  str[3] = rb_point_x(rb_line_start_point(self));
  str[4] = rb_point_y(rb_line_start_point(self));
  str[5] = rb_point_x(rb_line_end_point(self));
  str[6] = rb_point_y(rb_line_end_point(self));
  return rb_f_sprintf(i, str);
}

/*
 * call-seq:
 *   to_ary -> [start_point, end_point, width]
 *
 * Return an Array representing the line.
 */
VALUE rb_line_to_ary(VALUE self) {
  return rb_ary_new3(3, rb_line_start_point(self), rb_line_end_point(self), rb_line_width(self));
}

VALUE new_line_object(DELine line) {
  VALUE object = rb_line_allocate(rb_line_klass);
  *DELINE(object) = line;
  return object;
}

DELine* DELINE(VALUE object) {
  DELine *ptr;
  Data_Get_Struct(object, DELine, ptr);
  return ptr;
}

DELine VALUE_TO_DELINE(VALUE object) {
  if(rb_obj_is_kind_of(object, rb_line_class())) {
    return *DELINE(object);
  } else {
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(rb_line_class()));
  }
}
