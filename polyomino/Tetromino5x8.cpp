/*******************************************************************************

   Tetromino state space construction for exact_cover problem

   Author:  Richard Boyd
   email:   richardgboyd@gmail.com
   Date:    2013.07.30

*******************************************************************************/
#include "StateSpace.h"

int main(void)
{
   int i, j;
   Shape *temp;
   int boardState[] = { 
      1, 1, 1, 1, 1, 1, 1, 1,  
      1, 1, 1, 1, 1, 1, 1, 1,  
      1, 1, 1, 1, 1, 1, 1, 1,  
      1, 1, 1, 1, 1, 1, 1, 1,  
      1, 1, 1, 1, 1, 1, 1, 1  };  

   Board board(5, 8, boardState);
   Tetromino *tetrominos[10];
   StateSpace stateSpace;

   // Shapes for O
   tetrominos[0] = new Tetromino(Cell(0,0), Cell(1,0), Cell(0,1), Cell(1,1));
   tetrominos[1] = new Tetromino(tetrominos[0]);
   // Shapes for I
   tetrominos[2] = new Tetromino(Cell(0,0), Cell(1,0), Cell(2,0), Cell(3,0));
   tetrominos[3] = new Tetromino(tetrominos[2]);
   // Shapes for T
   tetrominos[4] = new Tetromino(Cell(0,0), Cell(1,0), Cell(2,0), Cell(1,1));
   tetrominos[5] = new Tetromino(tetrominos[4]);
   // Shapes for L
   tetrominos[6] = new Tetromino(Cell(0,0), Cell(0,1), Cell(0,2), Cell(1,2));
   tetrominos[7] = new Tetromino(tetrominos[6]);
   // Shapes for Z
   tetrominos[8] = new Tetromino(Cell(0,0), Cell(1,0), Cell(1,1), Cell(1,2));
   tetrominos[9] = new Tetromino(tetrominos[8]);

   // Add the board state
   stateSpace.setBoard(board);

   // Add all the tretominos to the state space
   for (i = 0; i < 10; i++)
      stateSpace.addShape(tetrominos[i]);

   // Add those tetrominos that we want to rotate
   // This is all but the first two O tetrominos
   for (i = 2; i < 4; i++)
      stateSpace.addShape(tetrominos[i]->rotate());

   // Add the remainder of the tetrominos that need a full
   // set of rotations, this is all but the I tetrominos
   for (i = 4; i < 10; i++)
   {
      temp = tetrominos[i]->rotate();
      for (j = 0; j < 3; j++)
      {
         stateSpace.addShape(temp);
         temp = temp->rotate();
      }
   }

   // Add the refelctions for the L and Z tetrominos
   for (i = 6; i < 10; i++)
   {
      temp = tetrominos[i]->reflect();
      for (j = 0; j < 4; j++)
      {
         stateSpace.addShape(temp);
         temp = temp->rotate();
      }
   }

   stateSpace.mapAllShapes();
   stateSpace.displayState();
}
