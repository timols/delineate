#include "deline.h"

/**
 * DELine Class
 *
 * This class represents a horizontal or vertical line given a
 * start point and end point.
 *
 * deLine is a constructor for the class.
 */
DELine deLine(const CvPoint2D32f start_point, const CvPoint2D32f end_point, float width) {
    DELine line;
    line.start_point = start_point;
    line.end_point = end_point;
    line.width = width;
    return line;
}

/**
 * DELineList Class
 *
 * This class represents a list of DELines
 *
 * deLineList is a constructor for the class.
 */
DELineList* deLineList(int initial_size) {
    if (initial_size < 0) {
      return NULL;
    }

    DELineList* list = malloc(sizeof(DELineList));
    if (list == NULL) {
      return NULL;
    }

    list->allocated = initial_size;
    list->length = 0;
    list->line_array = malloc(sizeof(DELine) * initial_size);

    if (list->line_array == NULL) {
      free(list);
      return NULL;
    }

    return list;
}

/**
 * A utility method to add DELines to a DELineList
 */
void addLineToList(DELineList* list, DELine line) {
    if (list->length < list->allocated) {
      list->line_array[list->length] = line;
      list->length++;
    } else {
      DELine* tmp = realloc(list->line_array, sizeof(DELine) * (list->allocated + 1));

      if (tmp == NULL) {
        exit(-1);
      } else {
        list->allocated++;
        list->line_array = tmp;
        addLineToList(list, line);
      }
    }
}

/**
 * A utility method to free DELineLists and their DELine arrays
 */
void deReleaseList(DELineList* list) {
  if ( list->line_array )
    free(list->line_array);

  free(list);
}
