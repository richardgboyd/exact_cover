#include <memory.h>
#include "Board.h"

void Board::shapeMap(int cols, int rows, int *_boardMap)
{
   int i;
   int j;

   maxCols = cols;
   maxRows = rows;
   boardMap = new int[maxCols * maxRows];
   boardIndex = new int[maxCols *maxRows];

   j = 0;
   for (i = 0; i < (maxCols * maxRows); i++)
   {
      boardMap[i] = _boardMap[i];

      if (boardMap[i])
         boardIndex[i] = j++;
      else
         boardIndex[i] = -1; 
   }
}

bool Board::inRange(int index)
{
   if ((index >= 0) && (index < (maxCols * maxRows)))
   {
      return true;
   }

   return false;
}

int Board::getCell(int index)
{
   if (inRange(index))
      return boardMap[index];

   return 0;
}

int Board::getIndex(int index)
{
   if (inRange(index))
      return boardIndex[index];

   return -1;
}

bool Board::isCellMapped(int col, int row)
{

   return getCell(col * getMaxCols() + row) != 0;
}

bool Board::canShapeMap(int col, int row, Shape &shape)
{
   int index;

   // Make sure shape is not outside the bounds of the board
   if (shape.getMaxCol() + col > maxCols)
      return false;
   
   if (shape.getMaxRow() + row > maxRows)
      return false;

   // Make sure the shape maps to the board parameters
   for (index = 0; index < shape.getSize(); index++)
   {
      if (isCellMapped(shape.getCell(index).getCol() + col,
                       shape.getCell(index).getRow() + row) != true)
      {
         return false;
      }
   }
   
   return true;
}


bool Board::mapShapeAt(int col, int row, Shape &shape, int *state)
{
   int index;
   int j;

   memset(state, 0, getMaxCols() * getMaxRows());

   if (canShapeMap(col, row, shape))
   {
      for (index = 0; index < shape.getSize(); index++)
      {
         j =  getMaxCols() 
            * (row + shape.getCell(index).getRow()) 
            + (col + shape.getCell(index).getCol());

         state[getIndex(j)] = 1; 
      }

      return true;
   }

   return false;
}
