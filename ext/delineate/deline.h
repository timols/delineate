#ifndef DELINE_H_INCLUDED
#define DELINE_H_INCLUDED

#include <cv.h>

/**
 * Line class that is responsible for storing
 * horizontal/vertical lines identified on a page
 */
struct DEline
{
    float width;
    CvPoint2D32f start_point;
    CvPoint2D32f end_point;
};
typedef struct DEline DELine;

struct DElinelist
{
    int allocated;
    int length;
    DELine* line_array;
};
typedef struct DElinelist DELineList;

DELine deLine(const CvPoint2D32f start_point, const CvPoint2D32f end_point, float width);
DELineList* deLineList(int initial_size);
void addLineToList(DELineList* list, DELine line);
void deReleaseList(DELineList* list);

#endif
