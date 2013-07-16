#ifndef DLIST_HEADER
#define DLIST_HEADER

#include "stddef.h"

/**
 * The dlist stucture that must be contained in the structure which will
 * be used for dlist linking.
 */
struct dlist 
{
   struct dlist * next;
   struct dlist * prev;
};

/**
 * Empty head elements point back to themselves
 */
#define dlist_init(e) \
{ \
   (e)->next = (e); \
   (e)->prev = (e); \
}

/**
 * This function only works for dlist head elements
 * The next and previous links are maintained for
 * re-insert operations on list elements.
 */
#define dlist_is_empty(e) \
   (((e)->next == (e)) && ((e)->prev == (e)))

/**
 * Macros so that you can follow Knuth's L[R[x]] operations
 */
#define DLIST_RIGHT(x) \
   (x)->next \

#define DLIST_LEFT(x) \
   (x)->prev \

/**
 * See abov notes on macros for L[x] and R[x]
 */
#define dlist_remove(e) \
{ \
   DLIST_LEFT (DLIST_RIGHT(e)) = DLIST_LEFT (e); \
   DLIST_RIGHT(DLIST_LEFT (e)) = DLIST_RIGHT(e); \
}

/**
 * Please note this is a dangerous operation since the element
 * is re-inserted into the postion that it previously occupied.
 * This may result in un-intended list constructions since can
 * result in the list pointing to previously removed elements.
 */
#define dlist_reinsert(e) \
{ \
   DLIST_RIGHT(DLIST_LEFT (e)) = (e); \
   DLIST_LEFT (DLIST_RIGHT(e)) = (e); \
}

/**
 * Insert the element e to the head of list h
 *
 * Note that since element e and list h are of the same type you can 
 * just as easily insert element to the right of any element called h
 */
#define dlist_insert_head(h, e) \
{ \
   DLIST_LEFT (e) = (h); \
   DLIST_RIGHT(e) = DLIST_RIGHT(h); \
   DLIST_LEFT(DLIST_RIGHT(h)) = (e); \
   DLIST_RIGHT(h) = (e); \
}

/**
 * Insert the element e to the tail of list h
 *
 * Note since element e and list h are of the same type you can just
 * as easily inert element to the left of any element called h
 */
#define dlist_insert_tail(h, e) \
{ \
   DLIST_RIGHT(e) = (h); \
   DLIST_LEFT (e) = DLIST_LEFT(h); \
   DLIST_RIGHT(DLIST_LEFT(h)) = (e); \
   DLIST_LEFT(h) = (e); \
}

/**
 * Get, not remove, the element to the right of element called h
 */
#define dlist_get_head(h) (DLIST_RIGHT(h))

/**
 * Get, not remove, the element to the left of element called h
 */
#define dlist_get_tail(h) (DLIST_LEFT (h))

/**
 * Get, not remove, the element to the right of element called h
 */
#define dlist_get_next(e) (DLIST_RIGHT(e))

/**
 * Get, not remove, the element to the left of element called h
 */
#define dlist_get_prev(e) (DLIST_LEFT (e))

/**
 * Given an element e and a structure definition s with a member offset m
 * Return the address of the object for which the dlist element e is
 * is contained.
 */
#define dlist_get_object(e, s, m) \
   ((void *)(((char *)(e)) - offsetof(s, m)))

#endif

