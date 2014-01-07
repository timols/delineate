#include "point.h"
/*
 * Document-class: Delineate::Point
 *
 * This class means one point on X axis Y axis.
 * X and Y takes the value of the Float.
 *
 * C structure is here, very simple.
 *   typdef struct CvPoint2D32f{
 *     float x;
 *     float y;
 *   }
 */
extern CvPoint2D32f* CVPOINT2D32F(VALUE object);
extern CvPoint2D32f VALUE_TO_CVPOINT2D32F(VALUE object);

VALUE rb_point_klass;

VALUE rb_point_class() {
  return rb_point_klass;
}

void define_ruby_point_class() {
  if(rb_point_klass)
    return;
  /*
   * delineate = rb_define_module("Delineate");
   *
   * note: this comment is used by rdoc.
   */
  VALUE delineate = rb_module_delineate();

  rb_point_klass = rb_define_class_under(delineate, "Point", rb_cObject);
  rb_define_alloc_func(rb_point_klass, rb_point_allocate);
  rb_define_singleton_method(rb_point_klass, "compatible?", RUBY_METHOD_FUNC(rb_point_compatible_q), 1);
  rb_define_private_method(rb_point_klass, "initialize", RUBY_METHOD_FUNC(rb_point_initialize), -1);
  rb_define_method(rb_point_klass, "x", RUBY_METHOD_FUNC(rb_point_x), 0);
  rb_define_method(rb_point_klass, "x=", RUBY_METHOD_FUNC(rb_point_set_x), 1);
  rb_define_method(rb_point_klass, "y", RUBY_METHOD_FUNC(rb_point_y), 0);
  rb_define_method(rb_point_klass, "y=", RUBY_METHOD_FUNC(rb_point_set_y), 1);
  rb_define_method(rb_point_klass, "==", RUBY_METHOD_FUNC(rb_point_equal), 1);
  rb_define_method(rb_point_klass, "to_s", RUBY_METHOD_FUNC(rb_point_to_s), 0);
  rb_define_method(rb_point_klass, "to_ary", RUBY_METHOD_FUNC(rb_point_to_ary), 0);
  rb_define_alias(rb_point_klass, "to_a", "to_ary");
}

/*
 * call-seq:
 *   combatible?(obj)
 *
 * Return compatibility to CvPoint2D32f. Return true if object have method #x and #y.
 *
 * For example.
 *   class MyPoint2D32f
 *     def x
 *       95.7
 *     end
 *     def y
 *       70.2
 *     end
 *   end
 *   mp = MyPoint2D32f.new
 *   CvPoint2D32f.compatible?(mp)  #=> true
 *   CvPoint2D32f.new(mp)          #=> same as CvPoint2D32f(95.7, 70.2)
 */
VALUE rb_point_compatible_q(VALUE klass, VALUE object) {
  return (rb_respond_to(object, rb_intern("x")) && rb_respond_to(object, rb_intern("y"))) ? Qtrue : Qfalse;
}

// static void rb_point_free(CvPoint2D32f* p) {
//   ruby_xfree(p);
// }

VALUE rb_point_allocate(VALUE klass) {
  CvPoint2D32f *ptr;
  return Data_Make_Struct(klass, CvPoint2D32f, 0, -1, ptr);
  // return Data_Wrap_Struct(klass, NULL, rb_point_free, ruby_xmalloc(sizeof(CvPoint2D32f)));
}

/*
 * call-seq:
 *   new
 *   new(obj)
 *   new(x, y)
 *
 * Create new 2D-coordinate, (x, y).
 *
 * new() is same as new(0.0, 0.0)
 *
 * new(obj) is same as new(obj.x.to_f, obj.y.to_f)
 */
VALUE rb_point_initialize(int argc, VALUE *argv, VALUE self) {
  VALUE obj, x, y;
  switch (argc) {
  case 0:
    break;
  case 1:
    obj = argv[0];
    if (rb_point_compatible_q(rb_point_klass, obj)) {
      CVPOINT2D32F(self)->x = NUM2DBL(rb_funcall(rb_funcall(obj, rb_intern("x"), 0), rb_intern("to_f"), 0));
      CVPOINT2D32F(self)->y = NUM2DBL(rb_funcall(rb_funcall(obj, rb_intern("y"), 0), rb_intern("to_f"), 0));
    }
    else {
      rb_raise(rb_eArgError, "object is not compatible %s.", rb_class2name(rb_point_klass));
    }
    break;
  case 2:
    x = argv[0], y = argv[1];
    CVPOINT2D32F(self)->x = NUM2DBL(x);
    CVPOINT2D32F(self)->y = NUM2DBL(y);
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 0..2)", argc);
  }
  return Qnil;
}

/*
 * Return parameter on x-axis.
 */
VALUE rb_point_x(VALUE self) {
  return rb_float_new(CVPOINT2D32F(self)->x);
}

/*
 * call-seq:
 *   x = val
 *
 * Set x-axis parameter, return self.
 */
VALUE rb_point_set_x(VALUE self, VALUE x) {
  CVPOINT2D32F(self)->x = NUM2DBL(x);
  return self;
}

/*
 * Return parameter on y-axis.
 */
VALUE rb_point_y(VALUE self) {
  return rb_float_new(CVPOINT2D32F(self)->y);
}

/*
 * call-seq:
 *   y = val
 *
 * Set y-axis parameter, return self.
 */
VALUE rb_point_set_y(VALUE self, VALUE y) {
  CVPOINT2D32F(self)->y = NUM2DBL(y);
  return self;
}

/*
 * call-seq:
 *   to_s -> "<Delineate::Point:(self.x,self.y)>"
 *
 * Return x and y by String.
 */
VALUE rb_point_to_s(VALUE self) {
  const int i = 4;
  VALUE str[i];
  str[0] = rb_str_new2("<%s:(%g,%g)>");
  str[1] = rb_str_new2(rb_class2name(CLASS_OF(self)));
  str[2] = rb_point_x(self);
  str[3] = rb_point_y(self);
  return rb_f_sprintf(i, str);
}

/*
 * call-seq:
 *   to_ary -> [x, y]
 *
 * Return x and y by Array.
 */
VALUE rb_point_to_ary(VALUE self) {
  return rb_ary_new3(2, rb_point_x(self), rb_point_y(self));
}

/*
 * call-seq:
 *    == -> true
 *
 * Compare point with other point, returns Boolean
 */
VALUE rb_point_equal(VALUE self, VALUE other_point) {
  CvPoint2D32f* this_pt  = CVPOINT2D32F(self);
  CvPoint2D32f* other_pt = CVPOINT2D32F(other_point);
  return (((this_pt->x == other_pt->x) && (this_pt->y == other_pt->y)) ? Qtrue : Qfalse);
}

VALUE new_point_object(CvPoint2D32f point) {
  VALUE object = rb_point_allocate(rb_point_klass);
  *CVPOINT2D32F(object) = point;
  return object;
}
