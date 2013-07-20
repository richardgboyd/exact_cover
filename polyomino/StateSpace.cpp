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

   if (shape->getIndex() >= maxShapeIndex)
      maxShapeIndex = shape->getIndex() + 1;
}


Shape *StateSpace::getShapeAt(int index)
{
    if ( (index < (int)shapes.size()) && (index >= 0))
    {
        return shapes[index];
    }

    return NULL;
}

int *StateSpace::getNewRow(Shape *shape)
{
    int i;
    int *stateSpaceRow; 
    int boardSize = board.getMaxCols() * board.getMaxRows();

    stateSpaceRow = new int[maxShapeIndex + boardSize];

    for (i = 0; i < (maxShapeIndex + boardSize); i++)
        stateSpaceRow[i] = 0;

    stateSpaceRow[shape->getIndex()] = 1;

    return stateSpaceRow;
}

int StateSpace::mapShape(Shape *shape)
{
   int row, col, startingRows;
   bool shapeMapped;
   int *stateSpaceRow = getNewRow(shape);

   startingRows = stateRows.size();
   for (row = 0; row < board.getMaxRows(); row++)
   {
      for (col = 0; col < board.getMaxCols(); col++)
      {
         shapeMapped = board.mapShapeAt(
               col, row, *shape, &stateSpaceRow[maxShapeIndex]);

         if (shapeMapped)
         {
            stateRows.push_back(stateSpaceRow);
            stateSpaceRow = getNewRow(shape);
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
        mapShape(shapes[i]);
    }
}

void StateSpace::displayState(void)
{
   int i, j, size;

   size = maxShapeIndex + (board.getMaxCols() * board.getMaxRows());

   for (i = 0; i < stateRows.size(); i++)
   {
      for (j = 0; j < size; j++)
      {
         printf("%d, ", stateRows[i][j]);
      }

      printf("\n");
   }

   printf("\n");
}
