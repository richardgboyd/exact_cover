#include <stdio.h>
#include <stdlib.h>
#include "StateSpace.h"

StateSpace::~StateSpace()
{
    Shape *shape;

    while (!shapes.empty()) 
    {
        shape = shapes.back();
        shapes.pop_back();
        delete shape;
    }
}

void StateSpace::setBoard(Board const &_board)
{
    board = _board;
}


void StateSpace::addShape(Shape *shape)
{
    shapes.push_back(shape);
}


Shape *StateSpace::getShapeAt(int index)
{
    if ( (index < (int)shapes.size()) && (index >= 0))
    {
        return shapes[index];
    }

    return NULL;
}

int *StateSpace::getNewRow(int index)
{
    int i;
    int *stateSpaceRow; 
    int boardSize = board.getMaxCols() * board.getMaxRows();

    stateSpaceRow = new int[shapes.size() + boardSize];

    for (i = 0; i < ((int)shapes.size() + boardSize); i++)
        stateSpaceRow[i] = 0;

    stateSpaceRow[index] = 1;

    return stateSpaceRow;
}

int StateSpace::mapShape(int index)
{
   int row, col, startingRows;
   bool shapeMapped;
   int *stateSpaceRow = getNewRow(index);

   startingRows = stateRows.size();
   for (row = 0; row < board.getMaxRows(); row++)
   {
      for (col = 0; col < board.getMaxCols(); col++)
      {
         shapeMapped = board.mapShapeAt(
               col, row, *shapes[index], &stateSpaceRow[shapes.size()]);

         if (shapeMapped)
         {
            stateRows.push_back(stateSpaceRow);
            stateSpaceRow = getNewRow(index);
         }
           
      }
   }

   delete stateSpaceRow;

   return stateRows.size() - startingRows;
}

void StateSpace::mapAllShapes(void)
{
    int i;

    for (i = 0; i < (int)shapes.size(); i++)
    {
        mapShape(i);
    }
}

void StateSpace::displayState(void)
{
   int i, j, size;

   size = getShapeCount() + (board.getMaxCols() * board.getMaxRows());

   for (i = 0; i < stateRows.size(); i++)
   {
      for (j = 0; j < size; j++)
      {
         printf("%d, ", stateRows[i][j]);
      }

      printf("\n");
   }
}
