#ifndef DELINEATE_H_INCLUDED
#define DELINEATE_H_INCLUDED

#include <ruby.h>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deline.h"
#include "point.h"
#include "line.h"
#include "detection.h"

VALUE rb_module_delineate();
void define_ruby_module();

#endif
