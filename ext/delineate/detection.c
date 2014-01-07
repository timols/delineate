#include "detection.h"

IplImage* source_image;

VALUE rb_open_image(VALUE module, VALUE file_name) {
  if ( source_image )
    cvReleaseImage( &source_image );

  source_image = cvLoadImage( StringValuePtr(file_name), 0 );
  if ( source_image )
    return Qtrue;

  return Qfalse;
}

VALUE rb_close_image(VALUE module) {
  if ( source_image ) {
    cvReleaseImage( &source_image );
    return Qtrue;
  }
  return Qfalse;
}

VALUE rb_draw_horizontal_lines(VALUE module, VALUE file_name, VALUE min_length, VALUE skip_gap) {
  CvMat* horizontal = detect_horizontal_lines(source_image, NUM2INT(min_length), NUM2INT(skip_gap));
  draw_horizontal_lines(horizontal, StringValuePtr(file_name));

  if ( horizontal )
    cvReleaseMat( &horizontal );

  return Qtrue;
}

VALUE rb_draw_vertical_lines(VALUE module, VALUE file_name, VALUE min_length, VALUE skip_gap) {
  CvMat* vertical = detect_vertical_lines(source_image, NUM2INT(min_length), NUM2INT(skip_gap));
  draw_vertical_lines(vertical, StringValuePtr(file_name));

  if ( vertical )
    cvReleaseMat( &vertical );

  return Qtrue;
}

VALUE rb_horizontal_lines(VALUE module, VALUE min_length, VALUE skip_gap) {
  CvMat* horizontal           = detect_horizontal_lines(source_image, NUM2INT(min_length), NUM2INT(skip_gap));
  DELineList* list_horizontal = find_horizontal_boxes(source_image, horizontal, NUM2INT(min_length), NUM2INT(skip_gap));

  cvReleaseMat( &horizontal );

  VALUE lines = rb_ary_new2(list_horizontal->length);
  for( int i = 0; i < list_horizontal->length; i++ ) {
      DELine line = list_horizontal->line_array[i];
      rb_ary_push(lines, new_line_object(line));
  }

  deReleaseList( list_horizontal );

  return lines;
}

VALUE rb_vertical_lines(VALUE module, VALUE min_length, VALUE skip_gap) {
  // In order to simplify the act of identifying vertical lines,
  // we rotate the source image by transposing the image matrix then
  // perform our horizontal line detection. The resultant matrix of lines
  // is then transposed back to get our results
  IplImage* rot = cvCreateImage( cvSize(source_image->height, source_image->width), source_image->depth, source_image->nChannels);
  cvTranspose(source_image, rot);
  CvMat* horizontal = detect_horizontal_lines(rot, NUM2INT(min_length), NUM2INT(skip_gap));
  CvMat* vertical   = cvCreateMat(horizontal->cols, horizontal->rows, horizontal->type);
  cvTranspose(horizontal, vertical);
  cvReleaseImage( &rot );
  cvReleaseMat( &horizontal );

  DELineList* list_vertical = find_vertical_boxes(source_image, vertical, NUM2INT(min_length), NUM2INT(skip_gap));
  cvReleaseMat( &vertical );

  VALUE lines = rb_ary_new2(list_vertical->length);
  for( int i = 0; i < list_vertical->length; i++ ) {
      DELine line = list_vertical->line_array[i];
      rb_ary_push(lines, new_line_object(line));
  }

  deReleaseList( list_vertical );

  return lines;
}

void extend_ruby_delineate_module() {
  VALUE delineate = rb_module_delineate();
  rb_define_singleton_method(delineate, "open_image", RUBY_METHOD_FUNC(rb_open_image), 1);
  rb_define_singleton_method(delineate, "close_image", RUBY_METHOD_FUNC(rb_close_image), 0);
  rb_define_singleton_method(delineate, "draw_horizontal_lines", RUBY_METHOD_FUNC(rb_draw_horizontal_lines), 3);
  rb_define_singleton_method(delineate, "draw_vertical_lines", RUBY_METHOD_FUNC(rb_draw_vertical_lines), 3);
  rb_define_singleton_method(delineate, "horizontal_lines", RUBY_METHOD_FUNC(rb_horizontal_lines), 2);
  rb_define_singleton_method(delineate, "vertical_lines", RUBY_METHOD_FUNC(rb_vertical_lines), 2);
}
