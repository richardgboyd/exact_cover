#include "stdio.h"
#include "unitTest.h"
#include "exact_cover.h"

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
   int index, j;
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
void unitTest_simpleColumnObjectSelection(void)
{
   struct column_object * column;
   
   unitTest_constructSimpleState(&test_data);

   column = column_object_selection(&test_data.head);

   unitTest_Equal(
      column, &test_data.columns[0],
      ("unitTest_simpleColumnObjectSelection: expected columns[0] to be selected.\n")
   );
}

/**
 * Covering test to confirm that the columns are covered correctly this
 * involves removing the covered column from the list and any matching rows
 * from the list.
 */
void unitTest_simpleColumnObjectCovering(void)
{
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
}

void unitTest_simpleColumnObjectUncovering(void)
{
   struct column_object * column;
   struct dlist         * element;
   char   test_buffer[sizeof(test_data)];

   column = &test_data.columns[0];
   unitTest_constructSimpleState(&test_data);
   memcpy(test_buffer, &test_data, sizeof(test_buffer));

   column_object_covering(column);
   unitTest_True(
      (memcmp(test_buffer, &test_data, sizeof(test_buffer)) != 0),
      ("unitTest_simpleColumnObjectUncovering: expected covering operation to change test data.\n")
   );

   column_object_uncover(column);
   unitTest_True(
      (memcmp(test_buffer, &test_data, sizeof(test_buffer)) == 0),
      ("unitTest_simpleColumnObjectUncovering: expected uncovering operation to restore test data.\n")
   );

   // Cover both columns
   column = &test_data.columns[0];
   column_object_covering(column);
   memcpy(test_buffer, &test_data, sizeof(test_buffer));

   column = &test_data.columns[1];
   column_object_covering(column);
   unitTest_True(
      dlist_is_empty(&test_data.head.column),
      ("unitTest_simpleColumnObjectUncovering: head list is not empty after second cover operation.\n")
   );

   unitTest_True(
      (memcmp(test_buffer, &test_data, sizeof(test_buffer)) != 0),
      ("unitTest_simpleColumnObjectUncovering: expected second covering operation to change test data.\n")
   );

   // Uncover the second column
   column_object_uncover(column);
   unitTest_True(
      (memcmp(test_buffer, &test_data, sizeof(test_buffer)) == 0),
      ("unitTest_simpleColumnObjectUncovering: expected second covering operation to change test data.\n")
   );

   // Uncover the first column
   column = &test_data.columns[0];
   column_object_uncover(column);
   unitTest_True(
      (memcmp(test_buffer, &test_data, sizeof(test_buffer)) != 0),
      ("unitTest_simpleColumnObjectUncovering: expected second covering operation to change test data.\n")
   );

   // Confirm we have restored the original buffer
   memcpy(test_buffer, &test_data, sizeof(test_buffer));
   unitTest_constructSimpleState(&test_data);
   unitTest_True(
      (memcmp(test_buffer, &test_data, sizeof(test_buffer)) == 0),
      ("unitTest_simpleColumnObjectUncovering: expected second covering operation to change test data.\n")
   );
}

/**
 * foo
 */
void unitTest_simpleStateSpaceSearch(void)
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
void unitTest_sampleColumnObjectSelection(void)
{
   struct column_object * column;
   
   unitTest_constructSampleState(&test_data);

   column = column_object_selection(&test_data.head);

   unitTest_Equal(
      column, &test_data.columns[0],
      ("unitTest_sampleColumnObjectSelection: expected columns[0] to be selected.\n")
   );
}

/**
 * Unimplemented
 */
void unitTest_sampleColumnObjectCovering(void)
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
void unitTest_sampleColumnObjectUncovering(void)
{
   struct data_object * output[6];
   unitTest_constructSampleState(&test_data);
   state_space_search(&test_data.head, output, 0);
}

int main(int argc, char **argv)
{
   // Simple setup
   if (unitTest_GetErrorCount() == 0) unitTest_simpleColumnObjectSelection();
   if (unitTest_GetErrorCount() == 0) unitTest_simpleColumnObjectCovering();
   if (unitTest_GetErrorCount() == 0) unitTest_simpleColumnObjectUncovering();
   if (unitTest_GetErrorCount() == 0) unitTest_simpleStateSpaceSearch();

   // More complex setup
   if (unitTest_GetErrorCount() == 0) unitTest_sampleColumnObjectSelection();
   if (unitTest_GetErrorCount() == 0) unitTest_sampleColumnObjectCovering();
   if (unitTest_GetErrorCount() == 0) unitTest_sampleColumnObjectUncovering();

   if (unitTest_LoggingEnabled())
      printf("%s completed with %d errors.\n", argv[0], unitTest_GetErrorCount());

   return unitTest_GetErrorCount();
}
