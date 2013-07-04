#include "gtest/gtest.h"
extern "C" {
#include "exact_cover.h"
}

#define MAX_COLUMNS  (7)
#define MAX_ROWS     (6)
#define MAX_DATA     ((MAX_COLUMNS) * (MAX_ROWS))

struct state_space
{
   struct list_links    head;
   struct column_object columns[MAX_COLUMNS];
   struct data_object   datum  [MAX_DATA];
} test_data;

const char * names[MAX_COLUMNS] = { "A", "B", "C", "D", "E", "F", "G" };

int simple_state_space[] = { 1, 0,
                             0, 1,
                             1, 1 };

int example_state_space[] = { 0, 0, 1, 0, 1, 1, 0,
                              1, 0, 0, 1, 0, 0, 1,
                              0, 1, 1, 0, 0, 1, 0,
                              1, 0, 0, 1, 0, 0, 0,
                              0, 1, 0, 0, 0, 0, 1,
                              0, 0, 0, 1, 1, 0, 1  };

TEST(ExactCoverInitializers, ListLinksInitialize)
{
   list_links_initialize(&test_data.head);

   ASSERT_TRUE(dlist_is_empty(&test_data.head.row_traversal));
   ASSERT_TRUE(dlist_is_empty(&test_data.head.column_traversal));
}

TEST(ExactCoverInitializers, ColumnInitializeSingleEntry)
{
   column_object_initialize(&test_data.columns[0], (char *)names[0]);
   ASSERT_TRUE(dlist_is_empty(&test_data.columns[0].link.row_traversal));
   ASSERT_TRUE(dlist_is_empty(&test_data.columns[0].link.column_traversal));
   ASSERT_EQ(0, test_data.columns[0].count);
   ASSERT_STREQ("A", test_data.columns[0].name);
}

TEST(ExactCoverInitializers, ColumnInitializeMultipleEntries)
{
   int index;

   for (index = 0; index < MAX_COLUMNS; index++)
      column_object_initialize(&test_data.columns[index], (char *)names[index]);

   for (index = 0; index < MAX_COLUMNS; index++)
   {
      ASSERT_TRUE(dlist_is_empty(&test_data.columns[index].link.row_traversal));
      ASSERT_TRUE(dlist_is_empty(&test_data.columns[index].link.column_traversal));
      ASSERT_EQ(0, test_data.columns[index].count);
      ASSERT_STREQ(names[index], test_data.columns[index].name);
   }
}

TEST(ExactCoverInitializers, RowInitializeOneEntry)
{
   data_object_initialize(&test_data.datum[0]);

   ASSERT_TRUE(dlist_is_empty(&test_data.datum[0].link.row_traversal));
   ASSERT_TRUE(dlist_is_empty(&test_data.datum[0].link.column_traversal));
   ASSERT_EQ(NULL, test_data.datum[0].column_handle);
   ASSERT_EQ(0, test_data.datum[0].id);
}

TEST(ExactCoverInitializers, RowInitializeMultipleEntries)
{
   int index;

   for (index = 0; index < MAX_DATA; index++)
      data_object_initialize(&test_data.datum[index]);

   for (index = 0; index < MAX_DATA; index++)
   {
      ASSERT_TRUE(dlist_is_empty(&test_data.datum[index].link.row_traversal));
      ASSERT_TRUE(dlist_is_empty(&test_data.datum[index].link.column_traversal));
      ASSERT_EQ(NULL, test_data.datum[index].column_handle);
      ASSERT_EQ(0, test_data.datum[index].id);
   }
}

/**
 * Take the simple route and initialize the entire state space object
 *
 * todo: with a few more parameters this looks like a nice utility function
 *       to include in the exact_cover.c file.
 */
void unitTest_initializeStateSpace(
   struct state_space * test_data
)
{
   int index;

   list_links_initialize(&test_data->head); 

   for (index = 0; index < MAX_COLUMNS; index++)
      column_object_initialize(&test_data->columns[index], (char *)names[index]);

   for (index = 0; index < MAX_DATA; index++)
      data_object_initialize(&test_data->datum[index]);
}

/**
 * We have tested the basic initialization we can now move on to the more
 * complex initializers or building an actual state space.
 */
TEST(ExactCoverInitializers, StateSpaceAddSingleColumn)
{
   unitTest_initializeStateSpace(&test_data);

   state_space_add_column(&test_data.head, &test_data.columns[0]);
   ASSERT_TRUE(dlist_is_empty(&test_data.head.row_traversal));
   ASSERT_FALSE(dlist_is_empty(&test_data.head.column_traversal));
   ASSERT_EQ(&test_data.columns[0], 
             dlist_get_object(dlist_get_next(&test_data.head.column_traversal), struct column_object, link.column_traversal));
   ASSERT_EQ(&test_data.columns[0], COLUMN_OBJECT_DOWN(&test_data.columns[0]));
}

TEST(ExactCoverInitializers, StateSpaceAddMultipleColumns)
{
   int index;
   struct column_object * column;
   struct dlist         * element;

   unitTest_initializeStateSpace(&test_data);

   for (index = 0; index < MAX_COLUMNS; index++)
      state_space_add_column(&test_data.head, &test_data.columns[index]);

   index = 0;
   element = dlist_get_next(&test_data.head.column_traversal);
   while (element != &test_data.head.column_traversal)
   {
      column = COLUMN_OBJECT_FROM_COLUMN_DLIST(element);
      element = dlist_get_next(element);

      ASSERT_EQ(&test_data.columns[index++], column);
   }
}

TEST(ExactCoverInitializers, StateSpaceAddSingleRow)
{
   unitTest_initializeStateSpace(&test_data);

   state_space_add_column(&test_data.head, &test_data.columns[0]);
   column_object_add_data(&test_data.columns[0], &test_data.datum[0], 1);
   // Check that the column object was updated as we expected
   ASSERT_EQ(1, COLUMN_OBJECT_COUNT(&test_data.columns[0]));
   ASSERT_EQ(OBJECT_DOWN(&test_data.columns[0]), OBJECT_ROW(&test_data.datum[0]));
   // Check that the data object is now a member of the column
   ASSERT_TRUE(dlist_is_empty(DATA_OBJECT_COLUMN(&test_data.datum[0])));
   ASSERT_FALSE(dlist_is_empty(DATA_OBJECT_ROW(&test_data.datum[0])));
   ASSERT_EQ(&test_data.columns[0], DATA_OBJECT_COLUMN_HANDLE(&test_data.datum[0]));
   ASSERT_EQ(1, test_data.datum[0].id);
}

TEST(ExactCoverInitializers, StateSpaceAddMultipleRows)
{
   int index;
   struct column_object * column;
   struct data_object   * data;
   struct dlist         * element;

   unitTest_initializeStateSpace(&test_data);

   state_space_add_column(&test_data.head, &test_data.columns[0]);
   for (index = 0; index < MAX_COLUMNS; index++)
      column_object_add_data(&test_data.columns[0], &test_data.datum[index], index + 1);
   // Check that the column object was updated as we expected
   ASSERT_EQ(index, COLUMN_OBJECT_COUNT(&test_data.columns[0]));
   ASSERT_EQ(OBJECT_DOWN(&test_data.columns[0]), OBJECT_ROW(&test_data.datum[0]));

   // Initialize the data to walk the list by all of its columns
   index = 0;
   element = dlist_get_next(&test_data.head.column_traversal);
   column = COLUMN_OBJECT_FROM_COLUMN_DLIST(element);
   element = OBJECT_DOWN(column);
   while (element != COLUMN_OBJECT_ROW(column))
   {
      data = DATA_OBJECT_FROM_ROW_DLIST(element);
      element = OBJECT_DOWN(data);

      // Check that the data object is now a member of the column
      ASSERT_EQ(&test_data.columns[0], DATA_OBJECT_COLUMN_HANDLE(&test_data.datum[index]));
      ASSERT_EQ(index + 1, test_data.datum[index].id);
      ASSERT_EQ(&test_data.datum[index++], data);
   }
}

/**
 * Given the test data object and an array of 1s
 * Construct a state space for the specified number of 
 * columns and rows.
 *
 * todo: this seems like a nice utility function to include in the exact_cover.c
 * todo: data could be represented with a bit array though an odd number of bits
 *       in the columns would make this anoying.
 */
void unitTest_constructStateSpace(
   struct state_space * test_data,
   int                * data,
   int                  columns,
   int                  rows
)
{
   int index; //, j;
   int column_index = 0;
   int data_index   = 0;

   struct data_object * previous_data;

   for (index = 0; index < columns; index++)
      state_space_add_column(&test_data->head, &test_data->columns[index]);

   for (index = 0; index < (columns * rows); index++)
   {
      column_index = index % columns;
      
      if (column_index == 0)
         previous_data = NULL;

      if (data[index] != 0)
      {
         column_object_add_data(
            &test_data->columns[column_index], 
            &test_data->datum[data_index],
            index  / columns
         );

         if (previous_data)
            data_object_add_neighbor(previous_data, &test_data->datum[data_index]);
         else
            previous_data = &test_data->datum[data_index];

         data_index++;
      }
   }
}

/**
 * Setup simple state data
 */
void unitTest_constructSimpleState(
   struct state_space * test_data
)
{
   unitTest_initializeStateSpace(test_data);
   unitTest_constructStateSpace(test_data, simple_state_space, 2, 3);
}

/**
 * Setup example state data
 */
void unitTest_constructSampleState(
   struct state_space * test_data
)
{
   unitTest_initializeStateSpace(test_data);
   unitTest_constructStateSpace(test_data, example_state_space, 7, 6);
}

/**
 * Simple selection test since both columns have the same number of 
 * active rows both are available for selection and the first encountered
 * is returned so we check to column 0 to be returned.
 */
TEST(ExactCoverSimple, ColumnObjectSelection)
{
   struct column_object * column;
   
   unitTest_constructSimpleState(&test_data);
   column = column_object_selection(&test_data.head);

   ASSERT_EQ(column, &test_data.columns[0]);
}

/**
 * Covering test to confirm that the columns are covered correctly this
 * involves removing the covered column from the list and any matching rows
 * from the list.
 */
TEST(ExactCoverSimple, ColumnObjectCovering)
{
#if 0
   int index;
   struct column_object * column;
   struct dlist         * element;

   column = &test_data.columns[0];
   unitTest_constructSimpleState(&test_data);

   column_object_covering(column);
   ASSERT_FALSE(dlist_is_empty(&test_data.head.column));

   index = 0;
   element = dlist_get_next(&test_data.head.column);
   while (element != &test_data.head.column)
   {
      column = COLUMN_OBJECT_FROM_COLUMN_DLIST(element);
      element = dlist_get_next(element);

      ASSERT_EQ(column, &test_data.columns[index++]);
   }

   column = &test_data.columns[1];
   column_object_covering(column);
   ASSERT_TRUE(dlist_is_empty(&test_data.head.column));

   element = dlist_get_next(&test_data.head.column);
   while (element != &test_data.head.column)
   {
      column = COLUMN_OBJECT_FROM_COLUMN_DLIST(element);
      element = dlist_get_next(element);

      ASSERT_NE(column, &test_data.columns[1]);
   }
#endif
}

TEST(ExactCoverSimple, ColumnObjectUncovering)
{
   struct column_object * column;
   char   test_buffer[sizeof(test_data)];

   column = &test_data.columns[0];
   unitTest_constructSimpleState(&test_data);
   memcpy(test_buffer, &test_data, sizeof(test_buffer));

   column_object_covering(column);
   ASSERT_TRUE(memcmp(test_buffer, &test_data, sizeof(test_buffer)) != 0);

   column_object_uncover(column);
   ASSERT_TRUE(memcmp(test_buffer, &test_data, sizeof(test_buffer)) == 0);

   // Cover both columns
   column = &test_data.columns[0];
   column_object_covering(column);
   memcpy(test_buffer, &test_data, sizeof(test_buffer));

   column = &test_data.columns[1];
   column_object_covering(column);
   ASSERT_TRUE(dlist_is_empty(&test_data.head.column_traversal));
   ASSERT_TRUE(memcmp(test_buffer, &test_data, sizeof(test_buffer)) != 0);

   // Uncover the second column
   column_object_uncover(column);
   ASSERT_TRUE(memcmp(test_buffer, &test_data, sizeof(test_buffer)) == 0);

   // Uncover the first column
   column = &test_data.columns[0];
   column_object_uncover(column);
   ASSERT_TRUE(memcmp(test_buffer, &test_data, sizeof(test_buffer)) != 0);

   // Confirm we have restored the original buffer
   memcpy(test_buffer, &test_data, sizeof(test_buffer));
   unitTest_constructSimpleState(&test_data);
   ASSERT_TRUE(memcmp(test_buffer, &test_data, sizeof(test_buffer)) == 0);
}


/**
 * foo
 */
TEST(ExactCoverSimple, StateSpaceSearch)
{
   struct data_object *output[3];
   unitTest_constructSimpleState(&test_data);
   state_space_search(&test_data.head, output, 0);
}

/**
 * Sample selection test since both columns have the same number of 
 * active rows both are available for selection and the first encountered
 * is returned so we check to column 0 to be returned.
 */
TEST(ExactCoverSample, ColumnObjectSelection)
{
   struct column_object * column;
   
   unitTest_constructSampleState(&test_data);
   column = column_object_selection(&test_data.head);

   ASSERT_EQ(column, &test_data.columns[0]);
}

/**
 * Unimplemented
 */
TEST(ExactCoverSample, ColumnObjectCovering)
{
#if 0
   struct column_object * column;
   struct dlist         * element;

   column = &test_data.columns[0];
   unitTest_constructSimpleState(&test_data);

   column_object_covering(column);
   unitTest_False(
      dlist_is_empty(&test_data.head.column),
      ("unitTest_simpleColumnObjectCovering: head list is empty after first cover operation.\n")
   );

   element = dlist_get_next(&test_data.head.column);
   while (element != &test_data.head.column)
   {
      column = COLUMN_OBJECT_FROM_COLUMN_DLIST(element);
      element = dlist_get_next(element);

      unitTest_NotEqual(
         column, &test_data.columns[0],
         ("unitTest_simpleColumnObjectCovering: column[0] not removed from list.\n")
      );
   }

   column = &test_data.columns[1];
   column_object_covering(column);

   unitTest_True(
      dlist_is_empty(&test_data.head.column),
      ("unitTest_simpleColumnObjectCovering: head list is not empty after second cover operation.\n")
   );

   element = dlist_get_next(&test_data.head.column);
   while (element != &test_data.head.column)
   {
      column = COLUMN_OBJECT_FROM_COLUMN_DLIST(element);
      element = dlist_get_next(element);

      unitTest_NotEqual(
         column, &test_data.columns[1],
         ("unitTest_simpleColumnObjectCovering: column[1] not removed from list.\n")
      );
   }
#endif
}

/**
 * Unimplemented
 */
TEST(ExactCoverSample, ColumnObjectUnCovering)
{
   struct data_object * output[6];
   unitTest_constructSampleState(&test_data);
   state_space_search(&test_data.head, output, 0);
}

