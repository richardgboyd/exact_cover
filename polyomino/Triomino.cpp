/*******************************************************************************

   Triomino state space construction for exact_cover problem

   Author:  Richard Boyd
   email:   richardgboyd@gmail.com
   Date:    2013.07.15

*******************************************************************************/
#include "StateSpace.h"

int main(void)
{
   Shape *temp;
   int boardState[] = { 1, 1, 1,  1, 1, 1,  1, 1, 1 };
   Board board(3, 3, boardState);
   Triomino *triominoI = new Triomino(Cell(0,0), Cell(1,0), Cell(2,0));
   Triomino *triominoV = new Triomino(Cell(0,0), Cell(1,0), Cell(0,1));
   StateSpace stateSpace;

   // Add the shapes for I and rotated I
   // Each should map three times for a total mapping of 6
   stateSpace.setBoard(board);
   stateSpace.addShape(triominoI);
   stateSpace.addShape(triominoI->rotate());

   // Add the shapes for V and all its rotations
   // Each should map four times for a total mapping of 16
   temp = triominoV;
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);

   stateSpace.mapAllShapes();
   stateSpace.displayState();
}
