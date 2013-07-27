#include "exact_cover.h"

#define MAX_COLUMNS  12
#define MAX_ROWS     38
#define MAX_DATA     (MAX_COLUMNS * MAX_ROWS)

struct state_space
{
   struct list_links    head;
   struct column_object columns[MAX_COLUMNS];
   struct data_object   datum  [MAX_DATA];
} triomino_data;

char *columnNames[] = { 
   "I", "V", "A", "0", "1", "2", "3", "4", "5", "6", "7", "8"
};

int triominoStateSpace[] = {
   1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 
   1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 
   1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 
   1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 
   1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 
   1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 
   0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 
   0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 
   0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 
   0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 
   0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 
   0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 
   0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 
   0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 
   0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 
   0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 
   0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 
   0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 
   0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 
   0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 
   0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 
   0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 
   0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 
   0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 
   0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 
   0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 
   0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 
   0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 
   0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 
   0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 
   0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 
   0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 
   0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 
   0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 
   0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 
   0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 
   0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 
   0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 
};

void triominoInitializeStateSpace(void)
{
   int i;

   list_links_initialize(&triomino_data.head); 

   for (i = 0; i < MAX_COLUMNS; i++)
      column_object_initialize(&triomino_data.columns[i], (char *)columnNames[i]);

   for (i = 0; i < MAX_DATA; i++)
      data_object_initialize(&triomino_data.datum[i]);
}


void triominoConstructStateSpace(
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

int main(void)
{
   struct data_object *output[MAX_COLUMNS];
   triominoInitializeStateSpace();
   triominoConstructStateSpace(&triomino_data, &triominoStateSpace[0], MAX_COLUMNS, MAX_ROWS);
   state_space_search(&triomino_data.head, output, 0);
}
