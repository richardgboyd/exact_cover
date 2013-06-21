#include "gtest/gtest.h"
#include "Board.h"

bool testBoardSpace(Board &board, int *stateSpace)
{
   int i;
   int spaceSize = board.getMaxCols() * board.getMaxRows();

   for (i = 0; i < spaceSize; i++)
   {
      if (stateSpace[i] != board.getCell(i))
         return false;
   }

   return true;
}

TEST(Board, defaultConstructor)
{
   Board board;

   ASSERT_EQ(board.getMaxCols(), 0);
   ASSERT_EQ(board.getMaxRows(), 0);
   ASSERT_EQ(board.getIndex(0), -1);
}

TEST(Board, mapShapeBasic)
{
   int testSpace[] = { 1, 1, 1, 1 };
   Board board;

   board.shapeMap(2, 2, testSpace);

   ASSERT_EQ(board.getMaxCols(), 2);
   ASSERT_EQ(board.getMaxRows(), 2);
   
   ASSERT_TRUE(testBoardSpace(board, testSpace));
}

TEST(Board, parameterizedConstructor)
{
   int testSpace[] = { 1, 1, 1, 1 };
   Board board(2, 2, testSpace);

   ASSERT_EQ(board.getMaxCols(), 2);
   ASSERT_EQ(board.getMaxRows(), 2);
   
   ASSERT_TRUE(testBoardSpace(board, testSpace));
}

TEST(Board, boardIndex)
{
   int testSpace[] = { 1, 0, 0, 1 };
   Board board(2, 2, testSpace);

   ASSERT_EQ(board.getMaxCols(), 2);
   ASSERT_EQ(board.getMaxRows(), 2);

   ASSERT_EQ(board.getIndex(0),  0);
   ASSERT_EQ(board.getIndex(1), -1);
   ASSERT_EQ(board.getIndex(2), -1);
   ASSERT_EQ(board.getIndex(3),  1);
}

TEST(Board, canMapBiomino1)
{
   int testSpace[] = { 1, 1, 1, 1 };
   Board board(2, 2, testSpace);
   Biomino biomino(Cell(0,0), Cell(0,1));

   ASSERT_TRUE(board.canShapeMap(0,0,biomino));
   ASSERT_TRUE(board.canShapeMap(1,0,biomino));
   ASSERT_FALSE(board.canShapeMap(0,1,biomino));
   ASSERT_FALSE(board.canShapeMap(1,1,biomino));
}

TEST(Board, canMapBiomino2)
{
   int testSpace[] = { 1, 1, 1, 1 };
   Board board(2, 2, testSpace);
   Biomino biomino(Cell(0,0), Cell(1,0));

   ASSERT_TRUE(board.canShapeMap(0,0,biomino));
   ASSERT_FALSE(board.canShapeMap(1,0,biomino));
   ASSERT_TRUE(board.canShapeMap(0,1,biomino));
   ASSERT_FALSE(board.canShapeMap(1,1,biomino));
}

TEST(Borad, canMapBiominoWithHole)
{
   int testSpace[] = { 1, 1, 1, 0 };
   Board board(2, 2, testSpace);
   Biomino biomino(Cell(0,0), Cell(0,1));

   ASSERT_TRUE (board.canShapeMap(0,0,biomino));
   ASSERT_FALSE(board.canShapeMap(1,0,biomino));
   ASSERT_FALSE(board.canShapeMap(0,1,biomino));
   ASSERT_FALSE(board.canShapeMap(1,1,biomino));
}

TEST(Board, mapBiominoSimple1)
{
   int testSpace[] = { 1, 1, 1, 1 };
   int stateSpace[4];
   int stateSpaceEmpty[] = { 0, 0, 0, 0 };
   int stateSpaceAt00[]  = { 1, 1, 0, 0 };
   int stateSpaceAt01[]  = { 0, 0, 1, 1 };
   Board board(2, 2, testSpace);
   Biomino biomino(Cell(0,0), Cell(1,0));

   memset(stateSpace, 0, sizeof(stateSpace));
   ASSERT_EQ(memcmp(stateSpace, stateSpaceEmpty, sizeof(stateSpace)), 0);

   ASSERT_TRUE(board.mapShapeAt(0, 0, biomino, stateSpace));
   ASSERT_EQ(memcmp(stateSpace, stateSpaceAt00, sizeof(stateSpace)), 0);

   memset(stateSpace, 0, sizeof(stateSpace));
   ASSERT_TRUE(board.mapShapeAt(0, 1, biomino, stateSpace));
   ASSERT_EQ(memcmp(stateSpace, stateSpaceAt01, sizeof(stateSpace)), 0);
}

TEST(Board, mapBiominoSimple2)
{
   int testSpace[] = { 1, 1, 1, 1 };
   int stateSpace[4];
   int stateSpaceEmpty[] = { 0, 0, 0, 0 };
   int stateSpaceAt00[]  = { 1, 0, 1, 0 };
   int stateSpaceAt01[]  = { 0, 1, 0, 1 };
   Board board(2, 2, testSpace);
   Biomino biomino(Cell(0,0), Cell(0,1));

   memset(stateSpace, 0, sizeof(stateSpace));
   ASSERT_EQ(memcmp(stateSpace, stateSpaceEmpty, sizeof(stateSpace)), 0);

   ASSERT_TRUE(board.mapShapeAt(0, 0, biomino, stateSpace));
   ASSERT_EQ(memcmp(stateSpace, stateSpaceAt00, sizeof(stateSpace)), 0);

   memset(stateSpace, 0, sizeof(stateSpace));
   ASSERT_TRUE(board.mapShapeAt(1, 0, biomino, stateSpace));
   ASSERT_EQ(memcmp(stateSpace, stateSpaceAt01, sizeof(stateSpace)), 0);
}

