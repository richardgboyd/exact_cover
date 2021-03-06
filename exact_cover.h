#ifndef EXACT_COVER_HEADER
#define EXACT_COVER_HEADER

#include "dlist.h"

/**
 * General object movement functions
 */
#define OBJECT_ROW_TRAVERSAL(o)  (&(o)->link.row_traversal)
#define OBJECT_COLUMN_TRAVERSAL(o) (&(o)->link.column_traversal)

#define OBJECT_RIGHT(o) dlist_get_next(OBJECT_COLUMN_TRAVERSAL(o))
#define OBJECT_LEFT(o)  dlist_get_prev(OBJECT_COLUMN_TRAVERSAL(o))

#define OBJECT_UP(o)   dlist_get_prev(OBJECT_ROW_TRAVERSAL(o))
#define OBJECT_DOWN(o) dlist_get_next(OBJECT_ROW_TRAVERSAL(o))


/**
 * Column object movement functions
 */
#if 0
#define COLUMN_OBJECT_RIGHT(c) \
   (struct column_object *)dlist_get_object( \
      OBJECT_RIGHT(c), struct column_object, link.column_traversal)

#define COLUMN_OBJECT_LEFT(c) \
   (struct column_object *)dlist_get_object( \
      OBJECT_LEFT(c), struct column_object, link.column_traversal)

#define COLUMN_OBJECT_UP(c) \
   (struct column_object *)dlist_get_object( \
      OBJECT_UP(c), struct column_object, link.row_traversal)
#endif

#define COLUMN_OBJECT_DOWN(c) \
   (struct column_object *)dlist_get_object( \
      OBJECT_DOWN(c), struct column_object, link.row_traversal)

#define COLUMN_OBJECT_FROM_ROW_TRAVERSAL(r) \
   (struct column_object *)dlist_get_object( \
      (r), struct column_object, link.row_traversal)

#define COLUMN_OBJECT_FROM_COLUMN_TRAVERSAL(c) \
   (struct column_object *)dlist_get_object( \
      (c), struct column_object, link.column_traversal)


#define COLUMN_OBJECT_ROW(c)    (&(c)->link.row_traversal)
#define COLUMN_OBJECT_COLUMN(c) (&(c)->link.column_traversal)
#define COLUMN_OBJECT_COUNT(c)  ((c)->count)
#define COLUMN_OBJECT_NAME(c)   ((c)->name)

/**
 * Data object utility macros
 */
#define DATA_OBJECT_RIGHT(d) \
   (struct data_object *)dlist_get_object( \
      OBJECT_RIGHT(d), struct data_object, link.column_traversal)

#define DATA_OBJECT_LEFT(d) \
   (struct data_object *)dlist_get_object( \
      OBJECT_LEFT(d), struct data_object, link.column_traversal)

#define DATA_OBJECT_UP(d) \
   (struct data_object *)dlist_get_object( \
      OBJECT_UP(d), struct data_object, link.row_traversal)

#define DATA_OBJECT_DOWN(d) \
   (struct data_object *)dlist_get_object( \
      OBJECT_DOWN(d), struct data_object, link.row_traversal)

#define DATA_OBJECT_FROM_ROW_TRAVERSAL(r) \
   (struct data_object *)dlist_get_object( \
      (r), struct data_object, link.row_traversal)

#define DATA_OBJECT_FROM_COLUMN_TRAVERSAL(c) \
   (struct data_object *)dlist_get_object( \
      (c), struct data_object, link.column_traversal)

#define DATA_OBJECT_ROW(d)           (&(d)->link.row_traversal)
#define DATA_OBJECT_COLUMN(d)        (&(d)->link.column_traversal)
#define DATA_OBJECT_COLUMN_HANDLE(d) ((d)->column_handle)

/**
 * List links has properties
 * LEFT[x], RIGHT[x], UP[x], DOWN[x]
 */
struct list_links
{
   struct dlist row_traversal;
   struct dlist column_traversal;
};

/**
 * column object x has properties
 * LEFT[x], RIGHT[x], UP[x], DOWN[x], name, count
 */
struct column_object
{
   struct list_links  link;
   int                count;
   char             * name;
};

/**
 * Data object x has properties
 * LEFT[x], RIGHT[x], UP[x], DOWN[x], COLUMN[x]
 */
struct data_object
{
   struct list_links      link;
   struct column_object * column_handle;
   int                    id;
};

/**
 * Initialization methods
 */
void list_links_initialize   (struct list_links    * object);
void data_object_initialize  (struct data_object   * object);
void column_object_initialize(struct column_object * object, char * name);


/**
 * Linking functions to create the initial state space
 */
void state_space_add_column(
   struct list_links    * head,
   struct column_object * object
);

void column_object_add_data(
   struct column_object * column, 
   struct data_object   * data,
   int                    index
);

void data_object_add_neighbor(
   struct data_object * object, 
   struct data_object * neighbor
);


struct column_object * column_object_selection(struct list_links * head);

void column_object_covering(struct column_object * c);
void column_object_uncover(struct column_object * c);

void state_space_search(
   struct list_links  *  head, 
   struct data_object ** O, 
   int                   k
);

#endif

