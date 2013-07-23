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
   int boardState[] = { 
      1, 1, 1, 1,  
      1, 1, 1, 1,  
      1, 1, 1, 1,  
      1, 1, 1, 1  };  
   Board board(4, 4, boardState);
   Tetromino *tetrominoI = new Tetromino(Cell(0,0), Cell(1,0), Cell(2,0), Cell(3,0));
   Tetromino *tetrominoO = new Tetromino(Cell(0,0), Cell(1,0), Cell(0,1), Cell(1,1));
   Tetromino *tetrominoT = new Tetromino(Cell(0,0), Cell(1,0), Cell(2,0), Cell(1,1));
   Tetromino *tetrominoL = new Tetromino(Cell(0,0), Cell(0,1), Cell(0,2), Cell(1,2));
   Tetromino *tetrominoZ = new Tetromino(Cell(0,0), Cell(1,0), Cell(1,1), Cell(1,2));
   StateSpace stateSpace;

   // Add the board state
   stateSpace.setBoard(board);
   // Add the shapes for I and rotated I
   stateSpace.addShape(tetrominoI);
   stateSpace.addShape(tetrominoI->rotate());
   // Add the shape O to the state space
   stateSpace.addShape(tetrominoO->rotate());

   // Add the shapes for T and all its rotations
   temp = tetrominoT;
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);

   // Add the rotations for L then reflect to J and add all rotations for J
   temp = tetrominoL;
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);

   temp = tetrominoL->reflect();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = tetrominoL->reflect();

   // Add the rotations for Z then reflect to S and add all rotations for S
   temp = tetrominoZ;
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);

   temp = tetrominoZ->reflect();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = temp->rotate();
   stateSpace.addShape(temp);
   temp = tetrominoL->reflect();

   stateSpace.mapAllShapes();
   stateSpace.displayState();
}
