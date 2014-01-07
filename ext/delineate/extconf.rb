#!/usr/bin/env ruby
require 'mkmf'

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']
HEADER_DIRS = [
  # First search /opt/local for macports
  '/opt/local/include',

  # Secondly search /usr/local/Cellar for homebrew
  '/usr/local/Cellar/opencv/2.4.3/include/opencv',
  '/usr/local/Cellar/opencv/2.4.2/include/opencv',

  # Then search /usr/local for people that installed from source
  '/usr/local/include',

  # Check the ruby install locations
  INCLUDEDIR,

  # Finally fall back to /usr
  '/usr/include',
]

LIB_DIRS = [
  # First search /opt/local for macports
  '/opt/local/lib',

 # Secondly search /usr/local/Cellar for homebrew
  '/usr/local/Cellar/opencv/2.4.3/lib',
  '/usr/local/Cellar/opencv/2.4.2/lib',

  # Then search /usr/local for people that installed from source
  '/usr/local/lib',

  # Check the ruby install locations
  LIBDIR,

  # Finally fall back to /usr
  '/usr/lib',
]

dir_config('opencv', HEADER_DIRS, LIB_DIRS)

opencv_headers   = ["cv.h", "highgui.h"]

opencv_libraries = ["opencv_calib3d", "opencv_contrib", "opencv_core", "opencv_features2d",
                    "opencv_flann", "opencv_gpu", "opencv_highgui", "opencv_imgproc",
                    "opencv_legacy", "opencv_ml", "opencv_objdetect", "opencv_video"]

opencv_libraries.each do |lib|
    raise "lib#{lib} not found." unless have_library(lib)
end

opencv_headers.each do |header|
  raise "#{header} not found." unless have_header(header)
end


have_header("point.h")
have_header("line.h")
have_header("delineate.h")
have_header("deline.h")

create_makefile('delineate/delineate')

