#include "stdio.h"
#include "limits.h"
#if defined(__cplusplus)
extern "C" {
#endif
#include "dlist.h"
#include "exact_cover.h"
#if defined(__cplusplus)
}
#endif

/**
 * Utility functions to make the exact cover objects easier
 * to initialize
 */
void list_links_initialize(struct list_links * object)
{
   dlist_init(&object->row_traversal);
   dlist_init(&object->column_traversal);
}

void data_object_initialize(struct data_object * object)
{
   list_links_initialize(&object->link);
   object->column_handle = NULL;
}

void column_object_initialize(struct column_object * object, char * name)
{
   list_links_initialize(&object->link);
   object->count = 0;
   object->name  = name;
}

/**
 * Linking functions to create the initial state space
 */
void state_space_add_column(
   struct list_links    * head, 
   struct column_object * object
)
{
   dlist_insert_tail(&head->column_traversal, COLUMN_OBJECT_COLUMN(object));
}

void column_object_add_data(
   struct column_object * column,
   struct data_object   * data,
   int                    index
)
{
   dlist_insert_tail(COLUMN_OBJECT_ROW(column), DATA_OBJECT_ROW(data));
   data->id = index;
   data->column_handle = column;
   column->count++;
}

void data_object_add_neighbor(
   struct data_object * object,
   struct data_object * neighbor
)
{
   //dlist_insert_tail(DATA_OBJECT_COLUMN(object), DATA_OBJECT_COLUMN(neighbor));
   dlist_insert_tail(&object->link.column_traversal, &neighbor->link.column_traversal);
}

/**
 * Select a column object for next operation
 *
 * for each j <- RIGHT[h], RIGHT[RIGHT[h]], ... while j != h
 *    if COUNT[j] < s then c <- j and s <- COUNT[j]
 */
struct column_object *
column_object_selection(struct list_links * head)
{
   int count = INT_MAX;

   struct column_object * selection = NULL;
   struct column_object * object;
   struct dlist         * element;

   element = dlist_get_next(&head->column_traversal);
   while (element != &head->column_traversal)
   {
      object = COLUMN_OBJECT_FROM_COLUMN_TRAVERSAL(element);

      if (object->count < count)
      {
         count = object->count;
         selection = object;
      }

      element = dlist_get_next(element);
   }

   return selection;
}

/**
 * Covering column C
 *
 * LEFT[RIGHT[c]] <- LEFT[c] and RIGHT[LEFT[c]] <- RIGHT[c]
 * for each i <- DOWN[c], DOWN[DOWN[c]], ... while c != i
 *    for each j <- RIGHT[i], RIGHT[RIGHT[i]]. ... while j != i
 *       UP[DOWN[j]] <- UP[j] and DOWN[UP[j]] <- DOWN[j]
 *       COUNT[COLUMN[j]] <- COUNT[COLUMN[j]] - 1
 *
 */
void
column_object_covering(struct column_object * c)
{
   struct dlist * row;
   struct dlist * column;

   struct data_object * object1;
   struct data_object * object2;

   // Unlink the column object from the list
   dlist_remove(&c->link.column_traversal);
   row = OBJECT_DOWN(c);

   while (row != &c->link.row_traversal)
   {
      object1 = DATA_OBJECT_FROM_ROW_TRAVERSAL(row);
      row = dlist_get_next(row);
      column = OBJECT_RIGHT(object1);

      while (column != DATA_OBJECT_COLUMN(object1))
      {
         object2 = DATA_OBJECT_FROM_COLUMN_TRAVERSAL(column);
         column = dlist_get_next(column);
         dlist_remove(DATA_OBJECT_ROW(object2));
         object2->column_handle->count--;
      }
   }
}

/**
 * Uncovering column c
 *
 * for each i <- UP[c], UP[UP[c]], ... while i != c
 *    for each j <- LEFT[i], LEFT[LEFT[i]], ... while j != i
 *       COUNT[COLUMN[j]] <- COUNT[COLUMN[j]] + 1
 *       UP[DOWN[j]] <- j and DOWN[UP[j]] <- j
 * LEFT[RIGHT[c]] <- c and RIGHT[LEFT[c]] <- c
 *
 */
void
column_object_uncover(struct column_object * c)
{
   struct dlist * row;
   struct dlist * column;

   struct data_object * object1;
   struct data_object * object2;

   row = OBJECT_UP(c);

   while (row != COLUMN_OBJECT_ROW(c))
   {
      object1 = DATA_OBJECT_FROM_ROW_TRAVERSAL(row);
      row = dlist_get_prev(row);
      column = OBJECT_LEFT(object1);

      while (column != DATA_OBJECT_COLUMN(object1))
      {
         object2 = DATA_OBJECT_FROM_COLUMN_TRAVERSAL(column);
         column = dlist_get_prev(column);
         object2->column_handle->count++;
         dlist_reinsert(DATA_OBJECT_ROW(object2));
      }
   }

   dlist_reinsert(&c->link.column_traversal);
}


/**
 * X
 *
 * if R[h] = h print current solution.
 * Otherwise choose a column object c (choose_column_object)
 * Cover column c
 * for each r <- DOWN[c], DOWN[DOWN[c]], ... while r != c
 *    set O(k) <- r
 *    for each j <- ROW[r], ROW[ROW[r]], ... while j != r
 *       Cover column C[j]
 *    search(k + 1)
 *    set r <- O(k) and c <- COLUMN[r]
 *    for each j <- LEFT[r], LEFT[LEFT[r]], ... while j != r
 *       uncover column C[j]
 * Uncover column c
 * return
 */
void
state_space_search(
   struct list_links  *  head, 
   struct data_object ** O, 
   int                   k
)
{
   struct dlist * x;
   struct dlist * r;
   struct dlist * j;
   struct column_object * c;
   struct data_object   * object1;
   struct data_object   * object2;

   x = dlist_get_next(&head->column_traversal);

   if (x == &head->column_traversal)
   {
      int index;

      printf("Solution:\n");
      for (index = 0; index < k; index++)
         printf("\"%s:%d\" ", O[index]->column_handle->name, O[index]->id);
      printf("\n\n");

      return;
   }

   c = column_object_selection(head);

   column_object_covering(c);

   r = OBJECT_DOWN(c);
   while (r != COLUMN_OBJECT_ROW(c))
   {
      O[k] = DATA_OBJECT_FROM_ROW_TRAVERSAL(r);

      j = OBJECT_RIGHT(O[k]);
      while (j != COLUMN_OBJECT_COLUMN(O[k]))
      {
         object2 = DATA_OBJECT_FROM_COLUMN_TRAVERSAL(j);
         j = dlist_get_next(j);
         column_object_covering(object2->column_handle);
      }

      state_space_search(head, O, k+1);

      object1 = O[k];
      r = DATA_OBJECT_ROW(object1);
      c = object1->column_handle;
      j = OBJECT_LEFT(object1);
      while (j != DATA_OBJECT_COLUMN(object1))
      {
         object2 = DATA_OBJECT_FROM_COLUMN_TRAVERSAL(j);
         j = dlist_get_prev(j);
         column_object_uncover(object2->column_handle);
      }

      r = dlist_get_next(r);
   }

   column_object_uncover(c);
}

