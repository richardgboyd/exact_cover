#include "gtest/gtest.h"
#include "StateSpace.h"

TEST(StateSpace, defaultConstructor)
{
	StateSpace stateSpace;
}

TEST(StateSpace, setEmptyBoard)
{
	StateSpace stateSpace;
	Board board;

	stateSpace.setBoard(board);
   ASSERT_EQ(stateSpace.getBoard().getMaxCols(), 0);
   ASSERT_EQ(stateSpace.getBoard().getMaxRows(), 0);
   ASSERT_EQ(stateSpace.getBoard().getIndex(0), -1);
}

TEST(StateSpace, setSimpleBoard)
{
   int testSpace[] = { 1, 1, 1, 1 };
   Board board(2, 2, testSpace);
	StateSpace stateSpace;

	stateSpace.setBoard(board);
   ASSERT_EQ(stateSpace.getBoard().getMaxCols(), 2);
   ASSERT_EQ(stateSpace.getBoard().getMaxRows(), 2);
   ASSERT_EQ(stateSpace.getBoard().getIndex(0),  0);
}

TEST(StateSpace, addShapeOnce)
{
   StateSpace stateSpace;
   Biomino * biomino = new Biomino(Cell(0,0), Cell(1,0));

   stateSpace.addShape(biomino);
   ASSERT_EQ(stateSpace.getShapeCount(), 1);
   ASSERT_EQ(stateSpace.getShapeAt(0), biomino);
   ASSERT_TRUE(stateSpace.getShapeAt(1) == NULL);
}

TEST(StateSpace, addShapeTwice)
{
   StateSpace stateSpace;
   Biomino *biominoOne = new Biomino(Cell(0,0), Cell(1,0));
   Biomino *biominoTwo = new Biomino(Cell(0,0), Cell(0,1));

   stateSpace.addShape(biominoOne);
   stateSpace.addShape(biominoTwo);
   ASSERT_EQ(stateSpace.getShapeCount(), 2);
   ASSERT_EQ(stateSpace.getShapeAt(0), biominoOne);
   ASSERT_EQ(stateSpace.getShapeAt(1), biominoTwo);
}

TEST(StateSpace, getNewRow)
{
   int *stateSpaceRow;
   int boardState[] = { 1, 1, 1, 1 };
   Board    board(2, 2, boardState);
   Biomino *biominoOne = new Biomino(Cell(0,0), Cell(1,0));
   Biomino *biominoTwo = new Biomino(Cell(0,0), Cell(0,1));
   StateSpace stateSpace;

   stateSpace.setBoard(board);
   stateSpace.addShape(biominoOne);
   stateSpace.addShape(biominoTwo);

   stateSpaceRow = stateSpace.getNewRow(0);
   ASSERT_TRUE(stateSpaceRow != NULL);
   ASSERT_EQ(stateSpaceRow[0], 1);
   ASSERT_EQ(stateSpaceRow[1], 0);
   delete stateSpaceRow;

   stateSpaceRow = stateSpace.getNewRow(1);
   ASSERT_TRUE(stateSpaceRow != NULL);
   ASSERT_EQ(stateSpaceRow[0], 0);
   ASSERT_EQ(stateSpaceRow[1], 1);
   delete stateSpaceRow;
};

void testStateSpaceRowCompare(int *stateRow, int *testRow, int size)
{
   int i;

   for (i = 0; i < size; i++)
      ASSERT_EQ(stateRow[i], testRow[i]);
}

void testStateSpaceRowPrint(int *row, int size)
{
   int i;
   printf("    ");

   for (i = 0; i < (size - 1); i++)
      printf("%d, ", row[i]);

   printf("%d\n", row[i]);
}

void testStateSpaceRowDisplay(int index, int *stateRow, int *testRow, int size)
{
   printf("StateSpaceRow(%d)\n", index);
   testStateSpaceRowPrint(stateRow, size);
   testStateSpaceRowPrint(testRow, size);
}

TEST(StateSpace, mapBiomino)
{
   int createdRows, currentRow, i;
   int boardState[] = { 1, 1, 1, 1 };
   int stateSpaceRows[4][6] = { 
         { 1, 0, 1, 1, 0, 0 },
         { 1, 0, 0, 0, 1, 1 },
         { 0, 1, 1, 0, 1, 0 },
         { 0, 1, 0, 1, 0, 1 } };

   Board    board(2, 2, boardState);
   Biomino *biominoOne = new Biomino(Cell(0,0), Cell(1,0));
   Biomino *biominoTwo = new Biomino(Cell(0,0), Cell(0,1));
   StateSpace stateSpace;

   currentRow = 0;
   stateSpace.setBoard(board);
   stateSpace.addShape(biominoOne);
   stateSpace.addShape(biominoTwo);

   createdRows = stateSpace.mapShape(0);
   for (i = 0; i < createdRows; i++)
   {
      /*
      testStateSpaceRowDisplay(
         currentRow + i, 
         stateSpace.getStateRow(currentRow + i), 
         stateSpaceRows[currentRow + i], 
         ARRAY_SIZE(stateSpaceRows[0]));
      */
      testStateSpaceRowCompare(
         stateSpace.getStateRow(currentRow + i), 
         stateSpaceRows[currentRow + i], 
         ARRAY_SIZE(stateSpaceRows[0]));
   }
   currentRow += createdRows;

   createdRows = stateSpace.mapShape(1);
   for (i = 0; i < createdRows; i++)
   {
      /*
      testStateSpaceRowDisplay(
         currentRow + i, 
         stateSpace.getStateRow(currentRow +i), 
         stateSpaceRows[currentRow + i], 
         ARRAY_SIZE(stateSpaceRows[0]));
      */
      testStateSpaceRowCompare(
         stateSpace.getStateRow(currentRow + i), 
         stateSpaceRows[currentRow + i], 
         ARRAY_SIZE(stateSpaceRows[0]));
   }
}

TEST(StateSpace, mapBiominos)
{
   int i;
   int boardState[] = { 1, 1, 1, 1 };
   int stateSpaceRows[4][6] = { 
   //      I1 I2  00 10 01 11
         { 1, 0,  1, 1, 0, 0 },
         { 1, 0,  0, 0, 1, 1 },
         { 0, 1,  1, 0, 1, 0 },
         { 0, 1,  0, 1, 0, 1 } };

   Board    board(2, 2, boardState);
   Biomino *biominoOne = new Biomino(Cell(0,0), Cell(1,0));
   Biomino *biominoTwo = new Biomino(Cell(0,0), Cell(0,1));
   StateSpace stateSpace;

   stateSpace.setBoard(board);
   stateSpace.addShape(biominoOne);
   stateSpace.addShape(biominoTwo);

   stateSpace.mapAllShapes();
   //stateSpace.displayState();

   for (i = 0; i < stateSpace.getStateRowCount(); i++)
   {
      testStateSpaceRowCompare(
         stateSpace.getStateRow(i), 
         stateSpaceRows[i], 
         ARRAY_SIZE(stateSpaceRows[0]));
   }
}

TEST(StateSpace, mapTriominos)
{
   int i;
   int boardState[] = { 1, 1, 1,  1, 1, 1,  1, 1, 1 };
   int stateSpaceRows[22][15] = {
   //   I1 I2 V1 V2 V3 V4  00 10 20  01 11 12  20 21 22
      { 1, 0, 0, 0, 0, 0,  1, 1, 1,  0, 0, 0,  0, 0, 0 },
      { 1, 0, 0, 0, 0, 0,  0, 0, 0,  1, 1, 1,  0, 0, 0 },
      { 1, 0, 0, 0, 0, 0,  0, 0, 0,  0, 0, 0,  1, 1, 1 },

      { 0, 1, 0, 0, 0, 0,  1, 0, 0,  1, 0, 0,  1, 0, 0 },
      { 0, 1, 0, 0, 0, 0,  0, 1, 0,  0, 1, 0,  0, 1, 0 },
      { 0, 1, 0, 0, 0, 0,  0, 0, 1,  0, 0, 1,  0, 0, 1 },

      { 0, 0, 1, 0, 0, 0,  1, 1, 0,  1, 0, 0,  0, 0, 0 },  
      { 0, 0, 1, 0, 0, 0,  0, 1, 1,  0, 1, 0,  0, 0, 0 },  
      { 0, 0, 1, 0, 0, 0,  0, 0, 0,  1, 1, 0,  1, 0, 0 },  
      { 0, 0, 1, 0, 0, 0,  0, 0, 0,  0, 1, 1,  0, 1, 0 },  

      { 0, 0, 0, 1, 0, 0,  1, 1, 0,  0, 1, 0,  0, 0, 0 },  
      { 0, 0, 0, 1, 0, 0,  0, 1, 1,  0, 0, 1,  0, 0, 0 },  
      { 0, 0, 0, 1, 0, 0,  0, 0, 0,  1, 1, 0,  0, 1, 0 },  
      { 0, 0, 0, 1, 0, 0,  0, 0, 0,  0, 1, 1,  0, 0, 1 },  

      { 0, 0, 0, 0, 1, 0,  0, 1, 0,  1, 1, 0,  0, 0, 0 },  
      { 0, 0, 0, 0, 1, 0,  0, 0, 1,  0, 1, 1,  0, 0, 0 },  
      { 0, 0, 0, 0, 1, 0,  0, 0, 0,  0, 1, 0,  1, 1, 0 },  
      { 0, 0, 0, 0, 1, 0,  0, 0, 0,  0, 0, 1,  0, 1, 1 },  

      { 0, 0, 0, 0, 0, 1,  1, 0, 0,  1, 1, 0,  0, 0, 0 },  
      { 0, 0, 0, 0, 0, 1,  0, 1, 0,  0, 1, 1,  0, 0, 0 },  
      { 0, 0, 0, 0, 0, 1,  0, 0, 0,  1, 0, 0,  1, 1, 0 },  
      { 0, 0, 0, 0, 0, 1,  0, 0, 0,  0, 1, 0,  0, 1, 1 }
   };

   Shape *temp;
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

   ASSERT_EQ(stateSpace.getShapeCount(), 6);

   stateSpace.mapAllShapes();
   //stateSpace.displayState();

   ASSERT_EQ(stateSpace.getStateRowCount(), 22);

   for (i = 0; i < stateSpace.getStateRowCount(); i++)
   {
      testStateSpaceRowCompare(
         stateSpace.getStateRow(i), 
         stateSpaceRows[i], 
         ARRAY_SIZE(stateSpaceRows[0]));
   }
}

