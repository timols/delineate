#ifndef LINE_DETECTION_H_INCLUDED
#define LINE_DETECTION_H_INCLUDED

#include "delineate.h"

void detect_lines(IplImage* source_image, int min_length, int skip_gap);
void draw_horizontal_lines(CvMat* horizontal, const char* filename);
void draw_vertical_lines(CvMat* vertical, const char* filename);
CvMat* detect_horizontal_lines(IplImage* source_image, int min_length, int skip_gap);
CvMat* detect_vertical_lines(IplImage* source_image, int min_length, int skip_gap);
DELineList* find_horizontal_boxes(const IplImage* source_image, CvMat* horizontal_lines, int min_length, int skip_gap);
DELineList* find_vertical_boxes(const IplImage* source_image, CvMat* vertical_lines, int min_length, int skip_gap);

#endif
