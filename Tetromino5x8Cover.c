#include "exact_cover.h"

#define MAX_COLUMNS  50
#define MAX_ROWS     583
#define MAX_DATA     (MAX_COLUMNS * MAX_ROWS)

struct state_space
{
   struct list_links    head;
   struct column_object columns[MAX_COLUMNS];
   struct data_object   datum  [MAX_DATA];
} tetromino_data;

char *columnNames[] = { 
   "O1", "O2", "I1", "I2", "T1", "T2", "L1", "L2", "V1", "V2", 
   "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
   "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
   "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
   "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", 
};

int tetrominoStateSpace[] = {
   #include "Tetromino5x8.data"
};

void tetrominoInitializeStateSpace(void)
{
   int i;

   list_links_initialize(&tetromino_data.head); 

   for (i = 0; i < MAX_COLUMNS; i++)
      column_object_initialize(&tetromino_data.columns[i], (char *)columnNames[i]);

   for (i = 0; i < MAX_DATA; i++)
      data_object_initialize(&tetromino_data.datum[i]);
}


void tetrominoConstructStateSpace(
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
   tetrominoInitializeStateSpace();
   tetrominoConstructStateSpace(
      &tetromino_data, &tetrominoStateSpace[0], MAX_COLUMNS, MAX_ROWS);
   state_space_search(&tetromino_data.head, output, 0);
}
