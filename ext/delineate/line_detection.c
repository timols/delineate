#include "line_detection.h"

int debug = 0;

void draw_horizontal_lines(CvMat* horizontal, const char* filename)
{
    cvSaveImage(filename, horizontal, 0);
    return;
}

void draw_vertical_lines(CvMat* vertical, const char* filename)
{
    cvSaveImage(filename, vertical, 0);
    return;
}

void detect_lines(IplImage* source_image, int min_length, int skip_gap)
{
    CvMat*      horizontal      = detect_horizontal_lines(source_image, min_length, skip_gap);
    CvMat*      vertical        = detect_vertical_lines(source_image, min_length, skip_gap);
    DELineList* list_horizontal = find_horizontal_boxes(source_image, horizontal, min_length, skip_gap);
    DELineList* list_vertical   = find_vertical_boxes(source_image, vertical, min_length, skip_gap);

    if (debug) {
        draw_horizontal_lines(horizontal, "debug_horizontal.png");
        draw_vertical_lines(vertical, "debug_vertical.png");
    }

    for( int i = 0; i < list_horizontal->length; i++ ) {
        DELine line = list_horizontal->line_array[i];
        printf("hl:%.0f:[%.1f, %.1f]:[%.1f, %.1f]\n",
            line.width, line.start_point.x, line.start_point.y, line.end_point.x, line.end_point.y
        );
    }

    for( int i = 0; i < list_vertical->length; i++ ) {
        DELine line = list_vertical->line_array[i];
        printf("vl:%.0f:[%.1f, %.1f]:[%.1f, %.1f]\n",
            line.width, line.start_point.x, line.start_point.y, line.end_point.x, line.end_point.y
        );
    }

    deReleaseList(list_horizontal);
    deReleaseList(list_vertical);

    if (horizontal)
        cvReleaseMat( &horizontal );

    if (vertical)
        cvReleaseMat( &vertical );
}

/**
 * Given a matrix containing a series of horizontal lines, identify all
 * valid lines.
 *
 * @param   source_image        The 8bit single channel greyscale source image for line detection
 * @param   horizontal_lines    A reference to a matrix containing the horizontal line data
 *
 * Returns a list (DELineList) containing an array of DELines.
 */
DELineList* find_horizontal_boxes(const IplImage* source_image, CvMat* horizontal_lines, int min_length, int skip_gap)
{
    CvSeq*        hough_lines = 0;
    CvMemStorage* storage     = cvCreateMemStorage(0);
    IplImage*     cannyImage  = cvCreateImage(cvGetSize(source_image), source_image->depth, source_image->nChannels);

    cvCanny(horizontal_lines, cannyImage, 50, 300, 3);
    hough_lines = cvHoughLines2( cannyImage, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 60, min_length, skip_gap );
    DELineList* list = deLineList(0);

    int i, total = hough_lines->total;
    for( i = 0; i < total; i += 2 ) {
        if ( i + 1 < total ) {
            CvPoint*     top_line  = (CvPoint*) cvGetSeqElem(hough_lines, i);
            CvPoint*     bot_line  = (CvPoint*) cvGetSeqElem(hough_lines, i + 1);
            CvPoint2D32f top_left  = cvPointTo32f(top_line[0]);
            CvPoint2D32f top_right = cvPointTo32f(top_line[1]);
            CvPoint2D32f bot_left  = cvPointTo32f(bot_line[0]);
            CvPoint2D32f bot_right = cvPointTo32f(bot_line[1]);
            float        length    = abs(top_right.x - bot_left.x);
            float        thickness = abs((top_left.y - bot_right.y) - 1.0f);

            if ( length > 0.0f && thickness > 0.0f ) {
                top_right.y = top_right.y - 1.0f;
                top_left.y  = top_left.y - 1.0f;
                DELine line = deLine(top_left, top_right, thickness);
                addLineToList(list, line);
            }
        }
    }

    cvReleaseImage( &cannyImage );

    if ( storage )
        cvReleaseMemStorage( &storage );

    if ( hough_lines )
        cvClearSeq( hough_lines );

    return list;
}

/**
 * Given a source image, iteratve over all pixels and draw all horizontal lines that meet a
 * minimum length and don't have too many gaps.
 *
 * @param source_image          The 8bit single channel greyscale source image for line detection
 * @param min_length            The minimum length of line to detect before drawing
 * @param skip_gap              Minimum distance between lines before drawing as separate line
 *
 * Returns a matrix (CvMat*) with the lines drawn on the source image.
 */
CvMat* detect_horizontal_lines(IplImage* source_image, int min_length, int skip_gap)
{
    int height    = source_image->height;
    int width     = source_image->width;
    CvMat* im_out = cvCreateMat(height, width, CV_8UC1);

    int pixel, prev_pixel, run_start, run_end;

    for( int y = 0; y < height; y++ )
    {
        uchar* inp = ((uchar*)(source_image->imageData + source_image->widthStep * y));
        prev_pixel = 255;
        run_start = -1;
        run_end = 0;

        for ( int x = 0; x < width; x++ ) {
            pixel = inp[x] > 128 ? 255 : 0; // Determine if grey pixel should be treated as black or white

            // if white(x-1) to black(x) then
            // | |#|
            //    ^
            if ( prev_pixel == 255 && pixel == 0 ) {

                // the last gap is too large, close out the previous line
                if ( run_end > 0 && (x - run_end - 1) > skip_gap ) {

                    // draw the old line,
                    // but only if it is greater than the length we want
                    if ( (run_end - run_start + 1) >= min_length ) {
                        cvLine(im_out, cvPoint(run_start, y), cvPoint(run_end, y), CV_RGB(255, 255, 255), 1, 8, 0);

                    }
                    // now start a new one
                    run_start = x;
                }

                if ( run_start == -1 ) {
                    run_start = x;
                }
            }

            // if black(x-1) to white(x) then
            // |#| |
            //  ^
            else if ( prev_pixel == 0 && pixel == 255 ) {
                run_end = x - 1;
            }

            // If we're on the last pixel take care of the special case
            if ( x == width - 1 ) {

                if ( pixel == 0 ) {
                    run_end = x;
                }

                if ( run_end > 0 ) {
                    if ( (run_end - run_start + 1) >= min_length ) {
                        cvLine(im_out, cvPoint(run_start, y), cvPoint(run_end, y), CV_RGB(255, 255, 255), 1, 8, 0);
                    }
                }
            }

            // setup for the next loop
            prev_pixel = pixel;
        }
    }
    return im_out;
}

/**
 * Given a matrix containing a series of vertical lines, identify all
 * valid lines.
 *
 * @param   source_image      The 8bit single channel greyscale source image for line detection
 * @param   vertical_lines    A reference to a matrix containing the vertical line data
 *
 * Returns a list (DELineList) containing an array of DELines.
 */
DELineList* find_vertical_boxes(const IplImage* source_image, CvMat* vertical_lines, int min_length, int skip_gap)
{
    CvSeq*        hough_lines = 0;
    CvMemStorage* storage     = cvCreateMemStorage(0);
    IplImage*     cannyImage  = cvCreateImage(cvGetSize(source_image), IPL_DEPTH_8U, 1);

    cvCanny(vertical_lines, cannyImage, 50, 200, 3);
    hough_lines = cvHoughLines2( cannyImage, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 70, min_length, skip_gap );
    DELineList* list = deLineList(0);

    int i, total = hough_lines->total;
    for( i = 0; i < total; i += 2 ) {
        if ( i + 1 < total ) {
            CvPoint*     top_line  = (CvPoint*) cvGetSeqElem(hough_lines, i);
            CvPoint*     bot_line  = (CvPoint*) cvGetSeqElem(hough_lines, i + 1);
            CvPoint2D32f top_left  = cvPointTo32f(top_line[0]);
            CvPoint2D32f top_right = cvPointTo32f(top_line[1]);
            CvPoint2D32f bot_left  = cvPointTo32f(bot_line[0]);
            CvPoint2D32f bot_right = cvPointTo32f(bot_line[1]);
            float        length    = abs(top_right.y - bot_left.y);
            float        thickness = abs(top_left.x - bot_right.x - 1.0f);

            if ( length > 0.0f && thickness > 0.0f ) {
                top_right.x = top_right.x - 1.0f;
                top_left.x  = top_left.x - 1.0f;
                DELine line = deLine(top_right, top_left, thickness);
                addLineToList(list, line);
            }
        }
    }

    cvReleaseImage( &cannyImage );

    if ( storage )
        cvReleaseMemStorage( &storage );

    if ( hough_lines )
        cvClearSeq( hough_lines );

    return list;
}

/**
 * Given a source image, iteratve over all pixels and draw all vertical lines that meet a
 * minimum length and don't have too many gaps.
 *
 * @param source_image          The 8bit single channel greyscale source image for line detection
 * @param min_length            The minimum length of line to detect before drawing
 * @param skip_gap              Minimum distance between lines before drawing as separate line
 *
 * Returns a matrix (CvMat*) with the lines drawn on the source image.
 */
CvMat* detect_vertical_lines(IplImage* source_image, int min_length, int skip_gap)
{
    int height = source_image->height;
    int width = source_image->width;

    CvMat* im_out = cvCreateMat(height, width, CV_8UC1);

    int pixel, prev_pixel, run_start, run_end;

    for( int x = 0; x < width; x++ ) {
        prev_pixel = 255;
        run_start = -1;
        run_end = 0;

        for ( int y = 0; y < height; y++ ) {
            uchar* inp = ((uchar*)(source_image->imageData + source_image->widthStep * y));
            pixel = inp[x] > 128 ? 255 : 0; // Determine if grey pixel should be treated as black or white

            // if white(x-1) to black(x) then
            // | |#|
            //    ^
            if ( prev_pixel == 255 && pixel == 0 ) {

                // the last gap is too large, close out the previous line
                if ( run_end > 0 && (y - run_end - 1) > skip_gap ) {

                    // draw the old line,
                    // but only if it is greater than the length we want
                    if ( (run_end - run_start + 1) >= min_length ) {
                        cvLine(im_out, cvPoint(x, run_start), cvPoint(x, run_end), CV_RGB(255, 255, 255), 1, 8, 0);

                    }
                    // now start a new one
                    run_start = y;
                }

                if ( run_start == -1 ) {
                    run_start = y;
                }
            }

            // if black(x-1) to white(x) then
            // |#| |
            //  ^
            else if ( prev_pixel == 0 && pixel == 255 ) {
                run_end = y - 1;
            }

            // If we're on the last pixel take care of the special case
            if ( y == width - 1 ) {

                if ( pixel == 0 ) {
                    run_end = y;
                }

                if ( run_end > 0 ) {
                    if ( (run_end - run_start + 1) >= min_length ) {
                        cvLine(im_out, cvPoint(x, run_start), cvPoint(x, run_end), CV_RGB(255, 255, 255), 1, 8, 0);
                    }
                }
            }

            // setup for the next loop
            prev_pixel = pixel;
        }
    }
    return im_out;
}

int main(int argc, char const *argv[])
{
    if (argc != 6) {
        printf("Usage: %s <in.png> <out> <debug=0|1> <line_length=0-50> <skip_gap=1,2,3>\n", argv[0]);
        return(1);
    }

    // Parse command line arguments and setup for processing
    const char* image_name = argv[1];
    // const char* output_name = argv[2];
    debug = atoi(argv[3]);
    int min_length = atoi(argv[4]);
    int skip_gap = atoi(argv[5]);

    // Open the image file and identify lines
    IplImage* image = cvLoadImage(image_name, 0);
    detect_lines(image, min_length, skip_gap);
    cvReleaseImage( &image );
    return 0;
}
