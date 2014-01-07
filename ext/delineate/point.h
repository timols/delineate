#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "delineate.h"

VALUE rb_point_class();

void define_ruby_point_class();

VALUE rb_point_compatible_q(VALUE klass, VALUE object);

VALUE rb_point_allocate(VALUE klass);
VALUE rb_point_initialize(int argc, VALUE *argv, VALUE self);
VALUE rb_point_x(VALUE self);
VALUE rb_point_set_x(VALUE self, VALUE x);
VALUE rb_point_y(VALUE self);
VALUE rb_point_set_y(VALUE self, VALUE y);
VALUE rb_point_equal(VALUE self, VALUE other_point);

VALUE rb_point_to_s(VALUE self);
VALUE rb_point_to_ary(VALUE self);

VALUE new_point_object(CvPoint2D32f point);

inline CvPoint2D32f* CVPOINT2D32F(VALUE object) {
  CvPoint2D32f *ptr;
  Data_Get_Struct(object, CvPoint2D32f, ptr);
  return ptr;
}

inline CvPoint2D32f VALUE_TO_CVPOINT2D32F(VALUE object) {
  if(rb_point_compatible_q(rb_point_class(), object)){
    return cvPoint2D32f(NUM2DBL(rb_funcall(object, rb_intern("x"), 0)),
                        NUM2DBL(rb_funcall(object, rb_intern("y"), 0)));
  } else {
    rb_raise(rb_eTypeError, "require %s or compatible object.", rb_class2name(rb_point_class()));
  }
}
#endif
